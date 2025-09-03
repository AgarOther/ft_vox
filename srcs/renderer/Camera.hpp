#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Shader.hpp"
#include "types.hpp"

#define PLAYER_SPEED 3.75f
#define FOG_START 0.5f
#define FOG_END 1.0f
#define FOG_COLOR_OVERWORLD 180.0f / 255.0f, 210.0f / 255.0f, 1.0f
#define FOG_COLOR_NETHER 30.0f / 255.0f, 0.0f, 0.0f
#define FOG_COLOR_THE_END 15.0f / 255.0f, 0.0f, 25.0f / 255.0f

class Camera
{
	public:
		Camera(int width, int height, glm::vec3 position);

		void		setupMatrix(const Shader & shader);
		void		setupFog(const Shader & shader, Environment environment);

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
		bool		hasGuiOn() const { return _guiOn; }
		bool		isLocked() const { return _locked; }
		bool		isFullscreen() const { return (_fullScreen); }
		uint8_t		getRenderDistance() const { return _renderDistance; }
		bool		isFogActive() const { return (_fogActive); }

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
		void		setGui(bool guiOn) { _guiOn = guiOn; }
		void		setLocked(bool lock) { _locked = lock; }
		void		setFullscreen(bool fullscreen) { _fullScreen = fullscreen; }
		void		setRenderDistance(uint8_t renderDistance) { _renderDistance = renderDistance; } 
		void		setFogActive(bool fogActive) { _fogActive = fogActive; } 
		void		setFogStart(float fogStart) { _fogStart = fogStart; } 
		void		setFogEnd(float fogEnd) { _fogEnd = fogEnd; } 
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
		bool		_guiOn;
		bool		_locked;
		bool		_fullScreen;
		uint8_t		_renderDistance;
		bool		_fogActive;
		float		_fogStart;
		float		_fogEnd;
		typedef std::unordered_map<Environment, glm::vec3>	FogColorMap;
		FogColorMap	_fogColors;
};
