#include "TransformComponent.h"

namespace Systems::Components
{
	const glm::vec3 TransformComponent::xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 TransformComponent::yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 TransformComponent::zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::mat4 TransformComponent::GetMatrix() const
	{
		glm::mat4 transformation(1.0f);
		transformation = glm::translate(transformation, Position);
		transformation = transformation * GetRotationMatrix();
		transformation = glm::scale(transformation, Scale);
		return transformation;
	}

	glm::mat4 TransformComponent::GetInvertedTranslationMatrix() const
	{
		auto radians = glm::radians(Rotation);
		glm::mat4 transformation(1.0f);
		transformation = glm::rotate(transformation, radians.z, zAxis);
		transformation = glm::rotate(transformation, radians.x, xAxis);
		transformation = glm::rotate(transformation, radians.y, yAxis);
		transformation = glm::translate(transformation, -Position);
		return transformation;
	}

	glm::mat4 TransformComponent::GetRotationMatrix() const
	{
		return glm::toMat4(glm::quat(glm::radians(Rotation)));
	}
}