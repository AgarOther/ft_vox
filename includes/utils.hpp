#pragma once

#include <string>
#include "Camera.hpp"
#include "Player.hpp"
#include "types.hpp"
#include "imgui/imgui.h"

extern DebugInfo g_DEBUG_INFO;
extern const unsigned long long WORLD_SEED;

void						handleExit(int code, const std::string & message);
void						debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
const						std::string getFileAsString(const char * name);
const						std::string getShaderAsString(std::string path);
GLFWwindow *				initWindow(int *width, int *height);
void						toggleFullscreen(GLFWwindow * window, Camera & camera);
std::vector<std::string>	ft_split(const std::string & str, char delimiter);
Object						getObjectFromFile(const std::string & filepath);
int							floorDiv(int value, int divisor);
void                        dispatchCommand(char * buffer, Player * player);

// ImGui
ImGuiIO & 					getImGuiIO(GLFWwindow * window);
void						showImGui(const ImGuiIO & io, Player * player, float deltaTime);
void						renderImGui();
void						shutdownImGui();

// Texture Atlas
TextureBuffer               loadUnique(const std::string & texturePath);
TextureBuffer               loadSideTop(const std::string & side, const std::string & topBottom);
TextureBuffer               loadSideTopBottom(const std::string & side, const std::string & top, const std::string & bottom);
