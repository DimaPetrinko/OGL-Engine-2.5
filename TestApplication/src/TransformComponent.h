#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct TransformComponent
{
	static const glm::vec3 xAxis;
	static const glm::vec3 yAxis;
	static const glm::vec3 zAxis;

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 GetMatrix();
	glm::mat4 GetInvertedTranslationMatrix();
};