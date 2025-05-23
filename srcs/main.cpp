/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/26 21:53:16 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GL/glew.h>
#include <imgui.h>

#include "utils/Utils.hpp"
#include "minecraft/Chunk.hpp"
#include "minecraft/BlockType.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Crosshair.hpp"

int	WIDTH = 0;
int HEIGHT = 0;
t_debug DEBUG_INFOS;

int	main()
{
	Utils::setupGlfw();

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "ft_vox", nullptr, nullptr);
	WIDTH = mode->width;
	HEIGHT = mode->height;
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

	const ImGuiIO &io = Utils::getImGuiIO(window);

	const Shader shader("block.vert", "block.frag", true);
	Crosshair crosshair;

	Camera camera(mode->width, mode->height, glm::vec3(0.0f, 0.0f, 0.0f));
	BlockType::init();

	Chunk *chunk = new Chunk(0, 0);
	// Chunk *chunk1 = new Chunk(15, 0);
	// Chunk *chunk2 = new Chunk(0, 15);
	// Chunk *chunk3 = new Chunk(15, 15);
	(void) chunk;
	// (void) chunk1;
	// (void) chunk2;
	// (void) chunk3;
	DEBUG_INFOS.blocks = 0;
	DEBUG_INFOS.drawCalls = 0;
	DEBUG_INFOS.triangles = 0;

	while (!glfwWindowShouldClose(window))
	{
		const bool hasGui = camera.hasGuiOn();
		DEBUG_INFOS.deltaTime = io.DeltaTime;
		camera.interceptInputs(window);
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			Utils::showImGui(io, camera);
		DEBUG_INFOS.blocks = 0;
		DEBUG_INFOS.drawCalls = 0;
		DEBUG_INFOS.triangles = 0;
		shader.use();
		camera.setupMatrix(shader);

		// Draw calls
		Chunk::renderAll();

		if (hasGui)
			Utils::renderImGui();
		crosshair.draw();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	shader.free();
	Chunk::deleteAll();
	Utils::shutdownImGui();
	BlockType::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
}
