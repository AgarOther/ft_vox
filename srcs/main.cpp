/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/23 17:50:03 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include "../includes/Utils.hpp"
#include "minecraft/BlockType.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Crosshair.hpp"
#include "minecraft/Chunk.hpp"

int	main()
{
	Utils::setupGlfw();

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "ft_vox", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);

	glewInit();
	glViewport(0, 0, mode->width, mode->height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	#ifdef ELEO_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif

	ImGuiIO &io = Utils::getImGuiIO(window);

	Shader shader("block.vert", "block.frag", true);
	Crosshair crosshair;

	Camera camera(mode->width, mode->height, glm::vec3(8.0f, 18.0f, 7.0f));
	BlockType::init();

	Chunk *chunk = new Chunk(0, 0);
	Chunk *chunk1 = new Chunk(15, 0);
	Chunk *chunk2 = new Chunk(0, 15);
	Chunk *chunk3 = new Chunk(15, 15);
	chunk->generate();
	chunk1->generate();
	chunk2->generate();
	chunk3->generate();
	
	while (!glfwWindowShouldClose(window))
	{
		bool hasGui = camera.hasGuiOn();
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			Utils::showImGui(io, camera);
		shader.use();
		camera.interceptInputs(window);
		camera.setupMatrix(shader);
		
		// Draw calls
		Chunk::drawAll();

		if (hasGui)
			Utils::renderImGui();
		crosshair.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.free();
	Utils::shutdownImGui();
	BlockType::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
}
