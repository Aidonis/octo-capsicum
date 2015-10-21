#include "nsfw.h"


// Each Case should set up a uniform using the pass parameters
bool nsfw::RenderPass::setUniform(const char *name, nsfw::UNIFORM::TYPE type, const void *value, unsigned count, bool normalize) 
{
	GLint location = glGetUniformLocation(*shader, name);

	switch (type)
	{
	case nsfw::UNIFORM::FLO1: //TODO_D("Setup float uniform!");
	{
		float val = *(GLfloat*)value;
		glUniform1f(location, *(GLfloat*)value);
		break;
	}
	case nsfw::UNIFORM::FLO3: //TODO_D("Setup vec3 uniform!");
		glUniform3fv(location, 1, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::FLO4: //TODO_D("Setup vec4 uniform!");
		glUniform4fv(location, 1, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::MAT4: //TODO_D("Setup mat4 uniform!");
		//std::to_string()
		std::cout << glm::to_string(*((glm::mat4*)value)) << std::endl;

		glUniformMatrix4fv(location, 1, false, (const GLfloat*)value);
		break;
	case nsfw::UNIFORM::INT1: //TODO_D("Setup integer uniform!");
		glUniform1i(location, *(GLint*)value);
		break;
	case nsfw::UNIFORM::TEX2: //TODO_D("Setup texture2D uniform!");
	{
		unsigned int val = *(GLuint*)value;
		glUniform1ui(location, *(GLuint*)value);
		break;
	}
	default:
		TODO_D("INVALID Uniform type.");
		break;
	}

	return false;
}

// set GL state settings and globally accessible uniforms! Should be called before rendering occurs!
void nsfw::RenderPass::prep(){
	
	TODO_D("OVERRIDE: Setup the state of the card for this pass.");
}

// reset the GL state- undo what we did just in case.
void nsfw::RenderPass::post() { TODO_D("OVERRIDE: Clean up the state of the card."); }