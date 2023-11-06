#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp"

struct Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	void MoveForward(float amt)
	{
		pos += forward * amt;
	}

	void MoveForwardN(float amt)
	{
		pos += glm::vec3(forward.x,0,forward.z) * amt;
	}


	void MoveRight(float amt)
	{
		pos += glm::cross(up, forward) * amt;
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));
	}

	void RotateY(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	}


	void operator*=(glm::mat3 R)
	{
		glm::vec3 N;
		N.x = forward.x*R[0][0] + forward.y*R[1][0] + forward.z*R[2][0];
		N.y = forward.x*R[0][1] + forward.y*R[1][1] + forward.z*R[2][1];
		N.z = forward.x*R[0][2] + forward.y*R[1][2] + forward.z*R[2][2];
		forward = N;
	}

protected:
private:
public:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};

#endif
