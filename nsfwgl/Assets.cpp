#include "nsfw.h"

using namespace nsfw::ASSET;

const char *nsfw::TYPE_NAMES[eSIZE + 1] = { "NONE","vao","ibo","vbo","tri-size","fbo","rbo","texture","shader","SIZE" };

#ifdef _DEBUG
#define ASSET_LOG(type) do { std::cerr << "Trying to create key: <" << TYPE_NAMES[type] << ">" << name << std::endl; } while(0)
#else 
#define ASSET_LOG(type) do {} while (0)
#endif


nsfw::GL_HANDLE nsfw::Assets::getVERIFIED(const AssetKey &key) const
{
#ifdef _DEBUG
			if (!handles.count(key))
			{
				std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
				return 0;
			}
#endif
			return handles.at(key);
}

bool nsfw::Assets::setINTERNAL(ASSET::GL_HANDLE_TYPE t, const char *name, GL_HANDLE handle)
{
	AssetKey key(t, name);
#ifdef _DEBUG
	if (handles.count(key))
	{
		std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
		return false;
	}
	else std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
	handles[key] = handle;
	return true;
}

unsigned nsfw::Assets::loadShaderFile(unsigned shaderType, const char* path){
	std::string shaderCode;
	//openfile
	std::ifstream stream(path);

	if(stream.is_open()){
		std::string line = "";
		while(std::getline(stream, line)){
			shaderCode += "\n" + line;
		}
		stream.close();
	}

	//convert to cstring 
	const char* shaderSourcePointer = shaderCode.c_str();

	//Create ID
	unsigned int shader = glCreateShader(shaderType);

	//Load Source code
	glShaderSource(shader, 1, &shaderSourcePointer, NULL);

	//compile shader
	glCompileShader(shader);

	//error check
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE){
		int infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

		const char* shaderName = NULL;
		switch(shaderType){
		case GL_VERTEX_SHADER:
			shaderName = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderName = "fragment";
			break;
		}
		fprintf(stderr, "Compile failure in %s shader:\n%s\n", shaderName, infoLog);
		delete[] infoLog;
	}
	return shader;


}

bool nsfw::Assets::makeVAO(const char * name, const struct Vertex *verts, unsigned vsize,  const unsigned * tris, unsigned tsize)
{
	//From CreateRenderObject
	//assign handles
	//handles[AssetKey(GL_HANDLE_TYPE::VBO, name)] = vbo
	//SetInternal
	ASSET_LOG(GL_HANDLE_TYPE::VBO);
	ASSET_LOG(GL_HANDLE_TYPE::IBO);
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);
	
	GLuint vao, ibo, vbo;

	//Generate Array + Buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	setINTERNAL(VAO, name, vao);
	setINTERNAL(IBO, name, ibo);
	setINTERNAL(VBO, name, vbo);
	setINTERNAL(GL_HANDLE_TYPE::SIZE, name, tsize);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize, verts, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ARRAY_BUFFER, tsize, tris, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //normal
	glEnableVertexAttribArray(2); //tangent
	glEnableVertexAttribArray(3); //UV Coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	//Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");
	return false;
}

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{
	ASSET_LOG(GL_HANDLE_TYPE::FBO);
	//TODO_D("Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.");

	GLuint fbo;
	//Setup Framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	

	GLenum attachPoint = GL_COLOR_ATTACHMENT0;
	std::vector<GLenum> drawBuffers;

	for (int i = 0; i < nTextures; i++){
		const unsigned depth = depths[i];
		const char* a_name = names[i];
		makeTexture(a_name, w, h, depth, nullptr);

		GL_HANDLE tex = get(TEXTURE, a_name);

		if(depth == GL_DEPTH_COMPONENT){
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, get(TEXTURE, names[i]), 0);
		}
		else{
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachPoint, GL_TEXTURE_2D, get(TEXTURE, names[i]), 0);
			drawBuffers.push_back(attachPoint);
			attachPoint++;
		}
	}

	glDrawBuffers(drawBuffers.size(), drawBuffers.data());


	//Error checking
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE){
		bool incompleteAttachment = status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
		bool invalidEnum = status == GL_INVALID_ENUM;
		bool invalidValue = status == GL_INVALID_VALUE;
		printf("Framebuffer Error!\n");
		return false;
	}
	setINTERNAL(FBO, name, fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const char *pixels)
{
	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	//TODO_D("Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.");
	GLuint tex;
	glGenTextures(1, &tex);
	setINTERNAL(TEXTURE, name, tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	GLenum a_depth = (depth == GL_DEPTH_COMPONENT) ? GL_DEPTH_ATTACHMENT : depth;

	glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLenum error = glGetError();
	if ( error != GL_NO_ERROR){
		bool invalidEnum = error == GL_INVALID_ENUM;
		bool invalidValue = error == GL_INVALID_VALUE;
		printf("error with texture.\n");
		glBindTexture(GL_TEXTURE_2D, 0);
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

bool nsfw::Assets::loadTexture(const char * name, const char * path)
{
	//TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");
	int imageWidth = 0;
	int imageHeight = 0;
	int imageFormat = 0;
	const char* data = (const char*)stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	switch(imageFormat){
	case 1: imageFormat = GL_RED; break;
	case 2: imageFormat = GL_RG; break;
	case 3: imageFormat = GL_RGB; break;
	case 4: imageFormat = GL_RGBA; break;
	}
	if( data == nullptr){
		std::cout << "error loading texture.\n" << stbi_failure_reason();
		return false;
	}

	makeTexture(name, imageWidth, imageHeight, imageFormat, data);
	stbi_image_free((void*)data);

	return true;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * fpath)
{
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	//TODO_D("Load shader from a file.");

	unsigned int vertex = loadShaderFile(GL_VERTEX_SHADER, vpath);
	if (vertex == 0 ){
		return false;
	}
	unsigned int fragment = loadShaderFile(GL_FRAGMENT_SHADER, fpath);
	if(fragment == 0 ){
		return false;
	}

	int success = GL_FALSE;
	GLuint shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if(success == GL_FALSE){
		int length = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(shader, length, 0, log);
		std::cout << "Error linking shader program.\n" << log << std::endl;
		delete[] log;
		return false;
	}

	setINTERNAL(SHADER, name, shader);
	return true;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	//name/meshName
	//name/textureName
	//WIP Make able to load multiple meshes / beware mesh names
	FBXFile file;
	file.load(path, FBXFile::UNITS_METER, false, false, false);
	//file.initialiseOpenGLTextures();

	//for (int i = 0; i < file.getTextureCount(); ++i)
	//{
	//	file.getTextureByIndex(i)->handle
	//}

	FBXMeshNode* mesh = file.getMeshByIndex(0);

	unsigned int vertSize, triSize;
	Vertex* verts = new Vertex[vertSize = mesh->m_vertices.size()];
	unsigned *tris = new unsigned[triSize = mesh->m_indices.size()];

	//Load Vert Data
	for (int i = 0; i < mesh->m_vertices.size(); i++){
		verts[i] = { mesh->m_vertices[i].position, mesh->m_vertices[i].normal, mesh->m_vertices[i].tangent, mesh->m_vertices[i].texCoord1 };
	}

	//LoadTri Data
	for (int i = 0; i < mesh->m_indices.size(); i++){
		tris[i] = mesh->m_indices[i];
	}

	//load textures
	for (int i = 0; i < file.getTextureCount(); i++)
	{
		FBXTexture* tex = file.getTextureByIndex(i);
		loadTexture(tex->name.c_str(), tex->path.c_str());
	}

	TODO_D("Load FBX textures! You can use fbx shit or not");

	makeVAO(name, verts, vertSize, tris, triSize);

	file.unload();


	//TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");
	return false;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)
{
	TODO_D("OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n");
	//TODOSetup Tiny OBJ
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	return false;
}

void nsfw::Assets::init()
{
	//TODO_D("Load up some default assets here if you want.");
	
	setINTERNAL(FBO,"Screen",0);
	
	makeVAO("Cube",CubeVerts,24,CubeTris,36);
	makeVAO("Quad",QuadVerts, 4, QuadTris,6);
	/*
	char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGB8, w);
	*/

}

void nsfw::Assets::term()
{
	TODO();
	for each(std::pair<AssetKey,unsigned> k in handles)
	{
		switch (k.first.first)
		{
		case VBO:		TODO_D("VBO deletion");		break;
		case IBO:		TODO_D("IBO deletion");		break;
		case VAO:		TODO_D("VAO deletion");		break;
		case SHADER:	TODO_D("Shader deletion");	break;
		case TEXTURE:	TODO_D("Texture deletion"); break;
		case RBO:		TODO_D("RBO deletion");		break;
		case FBO:		TODO_D("FBO deletion");		break;
		}
	}
}


