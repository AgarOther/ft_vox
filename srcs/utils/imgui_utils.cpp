/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imgui_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:22:03 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 23:09:43 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <GLFW/glfw3.h>

ImGuiIO &Utils::getImGuiIO(GLFWwindow *window)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	return (io);
}

void Utils::setupImGui(ImGuiIO &io, Camera &camera)
{
	static bool vsync = true;
	static bool vsyncChanged = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("ft_vox");
	
	ImGui::Text("Minecraft 0.00.1 (0.00.1/vanilla)");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	
	ImGui::NewLine();
	
	glm::vec3 camPos = camera.getPosition();
	ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "XYZ: %.3f / %.3f / %.3f", camPos.x, camPos.y, camPos.z);
	ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "Facing: (%.1f / %.1f)", camera.getYaw(), camera.getPitch());

	ImGui::Checkbox("VSync", &vsync);

	if (vsync != vsyncChanged)
	{
		glfwSwapInterval(vsync);
		vsyncChanged = vsync;
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
