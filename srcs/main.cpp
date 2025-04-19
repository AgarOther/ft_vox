/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 13:22:48 by scraeyme         ###   ########.fr       */
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

int	main(void)
{
	Utils::setupGlfw();

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glewInit();
	glViewport(0, 0, WIDTH, HEIGHT);

	#ifdef ELEO_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif

	// Vertices coordinates
	GLfloat vertices[] =
	{  //    Position         /      Color        /   TexCoord  /
		// Front face
		-0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 0.0f,
		 0.5f, 1.0f,  0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 1.0f,
		-0.5f, 1.0f,  0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 1.0f,
	
		// Back face
		 0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 0.0f,
		-0.5f, 1.0f, -0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 1.0f,
		 0.5f, 1.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 1.0f,
	
		// Left face
		-0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		-0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 0.0f,
		-0.5f, 1.0f,  0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 1.0f,
		-0.5f, 1.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 1.0f,
	
		// Right face
		 0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 0.0f,
		 0.5f, 1.0f, -0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 1.0f,
		 0.5f, 1.0f,  0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 1.0f,
	
		// Top face
		-0.5f, 1.0f,  0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		 0.5f, 1.0f,  0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 0.0f,
		 0.5f, 1.0f, -0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 1.0f,
		-0.5f, 1.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 1.0f,
	
		// Bottom face
		-0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   1.0f, 1.0f,
		-0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 1.0f
	};
	
	GLuint textureId[]
	{
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 0, 0, 0,
		2, 2, 2, 2
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		// Front face
		0, 1, 2,
		0, 2, 3,
	
		// Back face
		4, 5, 6,
		4, 6, 7,
	
		// Left face
		8, 9,10,
		8,10,11,
	
		// Right face
		12,13,14,
		12,14,15,
	
		// Top face
		16,17,18,
		16,18,19,
	
		// Bottom face
		20,21,22,
		20,22,23
	};

	Shader shader("test.vert", "test.frag");
	VAO vao;
	VBO vbo(vertices, sizeof(vertices));
	VBO texVbo(textureId, sizeof(textureId));

	// VAO has to be bound before EBO is made
	vao.bind();
	EBO ebo(indices, sizeof(indices));
	vao.linkAttribFloat(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), NULL);
	vao.linkAttribFloat(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	vao.linkAttribFloat(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	vao.linkAttribInt(texVbo, 3, 1, GL_UNSIGNED_INT, sizeof(GLuint), NULL);

	Utils::unbindAll();
	
	Texture nylium_top("textures/warped_nylium_top.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	nylium_top.setSlot(shader, "tex0", 0);
	nylium_top.bind();

	Texture nylium_side("textures/warped_nylium_side.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE);
	nylium_side.setSlot(shader, "tex1", 1);
	nylium_side.bind();

	Texture netherrack("textures/netherrack.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE);
	netherrack.setSlot(shader, "tex2", 2);
	netherrack.bind();
	
	glEnable(GL_DEPTH_TEST);

	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		camera.interceptInputs(window);
		camera.setupMatrix(120.0f, 0.1f, 100.0f, shader, "camMatrix");
		vao.bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vbo.free();
	vao.free();
	ebo.free();
	shader.free();
	nylium_top.free();
	nylium_side.free();
	netherrack.free();
	glfwDestroyWindow(window);
	glfwTerminate();
}
