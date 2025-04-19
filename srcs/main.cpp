/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 17:16:41 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	Shader shader("test.vert", "test.frag");
	// VAO has to be bound before EBO is made

	Block nylium(Material::WARPED_NYLIUM);
	Block crimson(Material::CRIMSON_NYLIUM);
	Block netherrack(Material::NETHERRACK);
	
	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	Utils::unbindAll();
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		camera.interceptInputs(window);
		camera.setupMatrix(120.0f, 0.1f, 100.0f, shader, "camMatrix");
		for (float x = 0; x < 16; x++)
			for (float y = 0; y < 16; y++)
				for (float z = 0; z < 16; z++)
				{
					if (y == 15)
					{
						if ((int)x % 2)
							nylium.placeBlockAt(Location(x, y, z), shader);
						else
							crimson.placeBlockAt(Location(x, y, z), shader);
					}
					netherrack.placeBlockAt(Location(x, y, z), shader);
				}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	nylium.free();
	netherrack.free();
	crimson.free();
	shader.free();
	glfwDestroyWindow(window);
	glfwTerminate();
}
