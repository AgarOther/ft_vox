/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imgui_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:22:03 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 13:06:14 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void Utils::toggleFullscreen(GLFWwindow *window, Camera &camera)
{
	static bool isFullscreen = false;
	static int windowPosX = 0;
	static int windowPosY = 0;
	static int windowWidth = 0;
	static int windowHeight = 0;

	isFullscreen = !isFullscreen;
	if (isFullscreen)
	{
		glfwGetWindowPos(window, &windowPosX, &windowPosY);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
		glfwSetWindowMonitor(window, nullptr, windowPosX, windowPosY, windowWidth, windowHeight, 0);
	camera.setFullscreen(isFullscreen);
}

ImGuiIO &Utils::getImGuiIO(GLFWwindow *window)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImFontConfig config;
	config.OversampleH = 3;
	config.OversampleV = 3;
	config.GlyphExtraAdvanceX = 0.8f;
	io.Fonts->AddFontFromFileTTF("resources/font/Minecraft.ttf", 14.0f, &config);
	io.Fonts->Build();

	ImGuiStyle &style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.1f);
	return (io);
}

static std::string getDirectionAsString(const float yaw)
{
    if (yaw >= -45 && yaw < 45)
        return "north";
    if (yaw >= 45 && yaw < 135)
        return "east";
    if (yaw >= -135 && yaw < -45)
        return "south";
    return "west";
}

static std::string getAxisDirectionAsString(const float yaw)
{
    if (yaw >= -45 && yaw < 45)
        return "positive Z";
    if (yaw >= 45 && yaw < 135)
        return "positive X";
    if (yaw >= 135 || yaw < -135)
        return "negative Z";
    return "negative X";
}

void Utils::showImGui(const ImGuiIO &io, Camera &camera)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("ft_vox");
	
	// FPS
	ImGui::Text("Minecraft 0.00.1 (0.00.1/vanilla)");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	
	ImGui::NewLine();
	// Position infos
	const glm::vec3 camPos = camera.getPosition();
	ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "XYZ: %.3f / %.3f / %.3f", camPos.x, camPos.y, camPos.z);
	ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "Facing: %s (Towards %s) (%.1f / %.1f)",
		getDirectionAsString(camera.getYaw()).c_str(), getAxisDirectionAsString(camera.getYaw()).c_str(), camera.getYaw(), camera.getPitch());
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.9f, 1.0f), "Blocks: %ld | Draw calls: %ld | Triangles: %ld",
		DEBUG_INFOS.blocks, DEBUG_INFOS.drawCalls, DEBUG_INFOS.triangles);

	ImGui::NewLine();

	// Speed
	float cameraSpeed = camera.getBaseSpeed();
	ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.0f, 15.0f);
	if (cameraSpeed != camera.getBaseSpeed())
		camera.setBaseSpeed(cameraSpeed);
	
	// FOV
	float FOV = camera.getFOV();
	ImGui::SliderFloat("FOV", &FOV, 30.0f, 120.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
	if (FOV != camera.getFOV())
	{
		camera.setFOV(FOV);
		camera.setupMatrix(Shader::getCurrentlyBoundShader());
	}

	// Sensitivity
	float sensitivity = camera.getSensitivity();
	ImGui::SliderFloat("Sensitivity", &sensitivity, 0.0f, 1000.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
	if (sensitivity != camera.getSensitivity())
		camera.setSensitivity(sensitivity);

	// Render Distance
	int farPlane = static_cast<int>(camera.getFarPlane());
	ImGui::SliderInt("Render Distance", &farPlane, 10, 300, "%d Blocks", ImGuiSliderFlags_AlwaysClamp);
	if (farPlane != static_cast<int>(camera.getFarPlane()))
		camera.setFarPlane(static_cast<float>(farPlane));

	// VSync
	static bool vsync = true;
	static bool vsyncChanged = false;
	ImGui::Checkbox("VSync", &vsync);
	if (vsync != vsyncChanged)
	{
		glfwSwapInterval(vsync);
		vsyncChanged = vsync;
	}

	// Key lock
	static bool lock = false;
	static bool lockChanged = false;
	ImGui::SameLine();
	ImGui::Checkbox("Lock keys", &lock);
	if (lock != lockChanged)
	{
		camera.setLocked(lock);
		lockChanged = lock;
	}

	ImGui::NewLine();
	ImGui::NewLine();
	static char buffer[100] = {};
	ImGui::InputText("Command line", buffer, 100);
	if (ImGui::Button("Enter"))
	{
		Utils::dispatchCommand(camera, buffer);
		buffer[0] = 0;
	}

	ImGui::End();
}

void Utils::renderImGui()
{
	ImGui::Render();
	ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Utils::shutdownImGui()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
