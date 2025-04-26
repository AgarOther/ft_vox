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
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "utils/Utils.hpp"
#include "minecraft/Block.hpp"
#include "minecraft/BlockType.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Crosshair.hpp"

int	WIDTH = 0;
int HEIGHT = 0;

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

	const Block test(Material::CHERRY_LOG, Location(0, 0, 0), 0, 0);
	
	while (!glfwWindowShouldClose(window))
	{
		const bool hasGui = camera.hasGuiOn();
		camera.interceptInputs(window);
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			Utils::showImGui(io, camera);
		shader.use();
		camera.setupMatrix(shader);
		test.place();

		if (hasGui)
			Utils::renderImGui();
		crosshair.draw();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	shader.free();
	Utils::shutdownImGui();
	BlockType::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
}
