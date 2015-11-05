#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

//Shadow Pass
class SPass : public nsfw::RenderPass{
	

public:
	SPass();

	void prep(){
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	}

	void draw(){
		
	}

	void post(){
		
	}


};