/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 23:39:48 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../includes/Utils.hpp"
#include "renderer/Shader.hpp"
#include "renderer/VAO.hpp"
#include "renderer/VBO.hpp"
#include "renderer/EBO.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Camera.hpp"
#include "renderer/TextureType.hpp"
#include "minecraft/Block.hpp"

int	main(void)
{
	Utils::setupGlfw();

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	#ifdef ELEO_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif

	ImGuiIO &io = Utils::getImGuiIO(window);

	Shader shader("block.vert", "block.frag");

	Block grassBlock(Material::GRASS_BLOCK);
	Block dirt(Material::DIRT);
	Block craftingTable(Material::CRAFTING_TABLE);
	
	Camera camera(WIDTH, HEIGHT, glm::vec3(8.0f, 18.0f, 7.0f));

	Utils::unbindAll();
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Utils::setupImGui(io, camera);
		shader.use();
		camera.interceptInputs(window);
		camera.setupMatrix(120.0f, 0.1f, 100.0f, shader, "camMatrix");
		for (float x = 0; x < 16; x++)
			for (float y = 0; y < 16; y++)
				for (float z = 0; z < 16; z++)
				{
					if (y == 15)
						grassBlock.placeBlockAt(Location(x, y, z));
					else
						dirt.placeBlockAt(Location(x, y, z));
				}
		craftingTable.placeBlockAt(Location(8.0f, 16.0f, 8.0f));
		Utils::renderImGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	grassBlock.free();
	dirt.free();
	craftingTable.free();
	shader.free();
	Utils::shutdownImGui();
	glfwDestroyWindow(window);
	glfwTerminate();
}
