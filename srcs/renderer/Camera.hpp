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

		void setupMatrix(const Shader & shader);

		// Getters
		glm::mat4	getViewMatrix() const { return _view; }
		glm::mat4	getProjectionMatrix() const { return _proj; }
		glm::vec3	getPosition() const { return _position; }
		glm::vec3	getOrientation() const { return _orientation; }
		glm::vec3	getAltitude() const { return _altitude; }
		int			getWidth() const { return _width; }
		int			getHeight() const { return _height; }
		float		getSpeed() const { return _speed; }
		float		getBaseSpeed() const { return _baseSpeed; }
		float		getSensitivity() const { return _sensitivity; }
		float		getYaw() const { return _yaw; }
		float		getPitch() const { return _pitch; }
		float		getFOV() const { return _FOV; }
		float		getFarPlane() const { return _farPlane; }
		bool		hasClicked() const { return _firstClick; }
		bool		hasGuiOn() const { return _guiOn; }
		bool		isLocked() const { return _locked; }
		bool		isFullscreen() const { return (_fullScreen); }
		uint8_t		getRenderDistance() const { return _renderDistance; }

		// Setters
		void		setPosition(const glm::vec3 &position) { _position = position; }
		void		setOrientation(const glm::vec3 &orientation) { _orientation = orientation; }
		void		setAltitude(const glm::vec3 &altitude) { _altitude = altitude; }
		void		setWidth(const int width) { _width = width; }
		void		setHeight(const int height) { _height = height; }
		void		setSpeed(const float s) { _speed = s; }
		void		setBaseSpeed(const float s) { _baseSpeed = s; }
		void		setSensitivity(const float s) { _sensitivity = s; }
		void		setYaw(const float s) { _yaw = s; }
		void		setPitch(const float s) { _pitch = s; }
		void		setFOV(const float s) { _FOV = s; }
		void		setFarPlane(const float farPlane) { _farPlane = farPlane; }
		void		setClicked(bool clicked) { _firstClick = clicked; }
		void		setGui(bool guiOn) { _guiOn = guiOn; }
		void		setLocked(bool lock) { _locked = lock; }
		void		setFullscreen(bool fullscreen) { _fullScreen = fullscreen; }
		void		setRenderDistance(uint8_t renderDistance) { _renderDistance = renderDistance; } 
	private:
		glm::mat4	_proj;
		glm::mat4	_view;
		glm::vec3	_position;
		glm::vec3	_orientation;
		glm::vec3	_altitude;
		int			_width;
		int			_height;
		float		_speed;
		float		_baseSpeed;
		float		_sensitivity;
		float		_yaw;
		float		_pitch;
		float		_FOV;
		float		_farPlane;
		bool		_firstClick;
		bool		_guiOn;
		bool		_locked;
		bool		_fullScreen;
		uint8_t		_renderDistance;
};
