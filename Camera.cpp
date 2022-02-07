#include "Camera.hpp"

namespace gps {

	//Camera constructor
	Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) : MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->cameraPosition = cameraPosition;
		this->cameraTarget = cameraTarget;
		this->cameraUpDirection = cameraUp;
		this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
		this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	//return the view matrix, using the glm::lookAt() function
	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	//return camera target
	glm::vec3 Camera::getCameraTarget()
	{
		return cameraTarget;
	}
	//update the camera internal parameters following a camera move event

	void Camera::move(MOVE_DIRECTION direction, float speed)
	{
		switch (direction) {
		case MOVE_FORWARD:
			cameraPosition = cameraPosition + cameraFrontDirection * speed;
			cameraTarget = cameraTarget + cameraFrontDirection * speed;
			break;

		case MOVE_RIGHT:
			cameraPosition = cameraPosition + cameraRightDirection * speed;
			cameraTarget = cameraTarget + cameraRightDirection * speed;
			break;

		case MOVE_LEFT:
			cameraPosition = cameraPosition - cameraRightDirection * speed;
			cameraTarget = cameraTarget - cameraRightDirection * speed;
			break;

		case MOVE_BACKWARD:
			cameraPosition = cameraPosition - cameraFrontDirection * speed;
			cameraTarget = cameraTarget - cameraFrontDirection * speed;
			break;
		}
	}
	//update the camera internal parameters following a camera rotate event
	//yaw - camera rotation around the y axis
	//pitch - camera rotation around the x axis
	void Camera::rotate(float yaw, float pitch, bool constrainPitch)
	{
		yaw = yaw * MouseSensitivity;
		Yaw = Yaw + yaw;

		pitch = pitch * MouseSensitivity;
		Pitch = Pitch + pitch;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch < -89.0f) {
				Pitch = -89.0f;
			}
			if (Pitch == 89.0f) {
				Pitch = 89.0;
			}
			if (Pitch > 89.0f) {
				Pitch = 89.0f;
			}
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		glm::vec3 frontVector;
		frontVector.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		frontVector.y = sin(glm::radians(Pitch));
		frontVector.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFrontDirection = glm::normalize(frontVector);
		cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
		cameraUpDirection = glm::normalize(glm::cross(cameraUpDirection, glm::vec3(0.0f, 0.0f, 1.0f)));
	}

	void Camera::scenePreview(float angle) {
		// set the camera
		//this->cameraPosition = glm::vec3(-9.0, 20.0, 48.0);
		this->cameraPosition = cameraPosition;

		// rotate with specific angle around Y axis
		glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));

		// compute the new position of the camera 
		// previous position * rotation matrix
		this->cameraPosition = glm::vec4(r * glm::vec4(this->cameraPosition, 1));
		this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
		cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
}