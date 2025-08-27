/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imgui_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:22:03 by scraeyme          #+#    #+#             */
/*   Updated: 2025/08/27 14:46:04 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "imgui/imgui.h"
#include "utils.hpp"
#include <GLFW/glfw3.h>
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

ImGuiIO & getImGuiIO(GLFWwindow * window)
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
	io.Fonts->AddFontFromFileTTF("assets/font/Minecraft.ttf", 14.0f, &config);
	io.Fonts->Build();

	ImGuiStyle &style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.3f);
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

void showImGui(const ImGuiIO & io, Player * player, float deltaTime)
{
	Camera * camera = player->getCamera();
	const Location & position = player->getLocation();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("ft_vox");
	
	// FPS
	ImGui::Text("Minecraft 0.00.1 (0.00.1/vanilla)");
	ImGui::Text("%.3f ms/frame (%.1f FPS) Delta: %.3f", 1000.0f / io.Framerate, io.Framerate, deltaTime);
	
	ImGui::NewLine();
	// Position infos
	ImGui::TextColored(ImVec4(0.0f, 0.6f, 0.8f, 1.0f), "Gamemode: %s", player->getGamemode() == CREATIVE ? "Creative" : "Survival");
	ImGui::TextColored(ImVec4(0.75f, 1.0f, 0.0f, 1.0f), "Velocity: %.3f / %.3f / %.3f", player->getVelocityX(), player->getVelocityY(), player->getVelocityZ());
	ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "XYZ: %.3f / %.3f / %.3f (Block: %d / %d / %d)",
		position.getX(), position.getY(), position.getZ(),
		static_cast<int>(std::floor(position.getX())), static_cast<int>(std::floor(position.getY())), static_cast<int>(std::floor(position.getZ())));
	ImGui::TextColored(ImVec4(1.0f, 0.74f, 0.0f, 1.0f), "Chunk XZ: %d / %d (%d / %d)",
		abs(static_cast<int>(camera->getPosition().x) % CHUNK_WIDTH), abs(static_cast<int>(camera->getPosition().z) % CHUNK_DEPTH),
		static_cast<int>(camera->getPosition().x / 16), static_cast<int>(camera->getPosition().z / 16));
	ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "Facing: %s (Towards %s) (%.1f / %.1f)",
		getDirectionAsString(camera->getYaw()).c_str(), getAxisDirectionAsString(camera->getYaw()).c_str(), camera->getYaw(), camera->getPitch());
	
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.9f, 1.0f), "Blocks: %ld | Draw calls: %ld | Triangles: %ld",
		g_DEBUG_INFO.blocks, g_DEBUG_INFO.drawCalls, g_DEBUG_INFO.triangles);

	ImGui::TextColored(ImVec4(0.4f, 0.84f, 1.0f, 1.0f), "Targeted block: %s", player->getTargetedBlock().blockType.name.c_str());

	// Speed
	float cameraSpeed = camera->getBaseSpeed();
	ImGui::SliderFloat("Player Speed", &cameraSpeed, 0.0f, 150.0f);
	if (cameraSpeed != camera->getBaseSpeed())
		camera->setBaseSpeed(cameraSpeed);

	// FOV
	float FOV = camera->getFOV();
	ImGui::SliderFloat("FOV", &FOV, 30.0f, 120.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
	if (FOV != camera->getFOV())
		camera->setFOV(FOV);

	// Sensitivity
	float sensitivity = camera->getSensitivity();
	ImGui::SliderFloat("Sensitivity", &sensitivity, 0.0f, 1000.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
	if (sensitivity != camera->getSensitivity())
		camera->setSensitivity(sensitivity);

	// Render Distance
	int renderDistance = camera->getRenderDistance();
	ImGui::SliderInt("Render Distance", &renderDistance, 2, 64, "%d Chunks", ImGuiSliderFlags_AlwaysClamp);
	if (renderDistance != camera->getRenderDistance())
		camera->setRenderDistance(static_cast<float>(renderDistance));

	// VSync
	static bool vsync = true;
	static bool vsyncChanged = false;
	ImGui::Checkbox("VSync", &vsync);
	if (vsync != vsyncChanged)
	{
		glfwSwapInterval(vsync);
		vsyncChanged = vsync;
	}

	// Gamemode
	static bool creative = player->getGamemode() == CREATIVE;
	static bool creativeChanged = false;
	ImGui::SameLine();
	ImGui::Checkbox("Creative", &creative);
	if (creative != creativeChanged)
	{
		player->setGamemode(creative ? CREATIVE : SURVIVAL);
		player->setVelocity(glm::vec3(0));
		creativeChanged = creative;
	}

	// Key lock
	static bool wireframe = false;
	static bool wireframeChanged = false;
	ImGui::SameLine();
	ImGui::Checkbox("Wireframe", &wireframe);
	if (wireframe != wireframeChanged)
	{
		g_DEBUG_INFO.wireframe = wireframe;
		wireframeChanged = wireframe;
	}

	// Key lock
	static bool chunkLock = false;
	static bool chunkLockChanged = false;
	ImGui::SameLine();
	ImGui::Checkbox("Lock Generation", &chunkLock);
	if (chunkLock != chunkLockChanged)
	{
		player->getWorld()->setProcedural(!chunkLock);
		chunkLockChanged = chunkLock;
	}

	ImGui::NewLine();
	ImGui::NewLine();
	static char buffer[100] = {};
	ImGui::InputText("Command line", buffer, 100);
	if (ImGui::Button("Enter"))
	{
		dispatchCommand(buffer, player);
		buffer[0] = 0;
	}
	ImGui::End();
}

void renderImGui()
{
	ImGui::Render();
	ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdownImGui()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
