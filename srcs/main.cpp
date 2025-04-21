/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 23:11:16 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../includes/Utils.hpp"
#include "renderer/Shader.hpp"
#include "renderer/VAO.hpp"
#include "renderer/VBO.hpp"
#include "renderer/EBO.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Camera.hpp"
#include "renderer/TextureType.hpp"
#include "minecraft/Chunk.hpp"

int	main(void)
{
	Utils::setupGlfw();

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	ImGuiIO &io = Utils::getImGuiIO(window);

	Shader shader("block.vert", "block.frag");
	
	Camera camera(WIDTH, HEIGHT, glm::vec3(8.0f, 18.0f, 7.0f));
	BlockType::init();

	Chunk chunk(0, 0);
	
	while (!glfwWindowShouldClose(window))
	{
		bool hasGui = camera.hasGuiOn();
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			Utils::showImGui(io, camera, window);
		shader.use();
		camera.interceptInputs(window);
		camera.setupMatrix(camera.getFOV(), 0.1f, 100.0f, shader, "camMatrix");
		
		// Draw calls
		chunk.draw();
		
		if (hasGui)
			Utils::renderImGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.free();
	Utils::shutdownImGui();
	BlockType::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();
}
