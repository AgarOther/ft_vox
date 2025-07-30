#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Shader.hpp"

class Camera
{
	public:
		Camera(int width, int height, glm::vec3 position);
		~Camera();

		void setupMatrix(const Shader & shader);
		void interceptInputs(GLFWwindow * window, float deltaTime);

		// Getters
		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;
		glm::vec3 getPosition() const;
		glm::vec3 getOrientation() const;
		glm::vec3 getAltitude() const;
		int getWidth() const;
		int getHeight() const;
		float getSpeed() const;
		float getBaseSpeed() const;
		float getSensitivity() const;
		float getYaw() const;
		float getPitch() const;
		float getFOV() const;
		float getFarPlane() const;
		bool hasClicked() const;
		bool hasGuiOn() const;
		bool isLocked() const;
		bool isFullscreen() const;

		// Setters
		void setPosition(const glm::vec3 &position);
		void setOrientation(const glm::vec3 &orientation);
		void setAltitude(const glm::vec3 &Altitude);
		void setWidth(int width);
		void setHeight(int height);
		void setSpeed(float s);
		void setBaseSpeed(float s);
		void setSensitivity(float s);
		void setYaw(float s);
		void setPitch(float s);
		void setFOV(float s);
		void setFarPlane(float farPlane);
		void setClicked(bool clicked);
		void setGui(bool guiOn);
		void setLocked(bool lock);
		void setFullscreen(bool fullScreen);
	private:
		glm::mat4 _proj;
		glm::mat4 _view;
		glm::vec3 _position;
		glm::vec3 _orientation;
		glm::vec3 _altitude;
		int _width;
		int _height;
		float _speed;
		float _baseSpeed;
		float _sensitivity;
		float _yaw;
		float _pitch;
		float _FOV;
		float _farPlane;
		bool _firstClick;
		bool _guiOn;
		bool _locked;
		bool _fullScreen;
};
