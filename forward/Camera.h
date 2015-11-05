#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "../nsfwgl/nsfw.h"

struct Camera
{
	glm::mat4 transform;
	float nearPlane, farPlane, aspect, fov;

	Camera() : nearPlane(1), farPlane(100), aspect(800.f/600.f), fov(90) {}

	virtual void update(){}
	void lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) { transform = glm::inverse(glm::lookAt(pos, target, up)); }
	glm::mat4 getView()		  const { return glm::inverse(transform); }
	glm::mat4 getProjection() const { return glm::perspective(fov, aspect, nearPlane, farPlane); }
};

struct FlyCamera : public Camera
{
	float speed;

	void update();
};