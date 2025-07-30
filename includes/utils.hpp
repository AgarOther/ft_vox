#pragma once

#include <string>
#include "Camera.hpp"
#include "types.hpp"
#include "imgui/imgui.h"

void						handleExit(int code, const std::string & message);
void						debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
const						std::string getFileAsString(const char * name);
const						std::string getShaderAsString(std::string path);
GLFWwindow *				initWindow(int *width, int *height);
void						toggleFullscreen(GLFWwindow * window, Camera & camera);
std::vector<std::string>	ft_split(const std::string & str, char delimiter);
Object						getObjectFromFile(const std::string & filepath);
void						voxLog(const std::string & str);

// ImGui
ImGuiIO & 					getImGuiIO(GLFWwindow * window);
void						showImGui(const ImGuiIO & io, Camera & camera);
void						renderImGui();
void						shutdownImGui();