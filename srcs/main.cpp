/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:15:58 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 00:02:30 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <glad/glad.h>
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

int	main(void)
{
	Utils::setupGlfw();

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);

	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};
	
	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	Shader shader ("test.vert", "test.frag");
	VAO vao;
	VBO vbo(vertices, sizeof(vertices));
	// VAO has to be bound before EBO is made
	vao.bind();
	EBO ebo(indices, sizeof(indices));
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), NULL);
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	vao.linkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

	GLuint uniID = glGetUniformLocation(shader.getId(), "scale");

	Texture texture("textures/warped_nylium_top.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.setSlot(shader, "tex0", 0);
	
	Utils::unbindAll();

	float rotation = 0.0f;
	double previousTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		double currentTime = glfwGetTime();
		if (currentTime - previousTime >= 1 / 60)
		{
			rotation += 0.2f;
			previousTime = currentTime;
		}

		// Initializing matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(90.0f), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shader.getId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shader.getId(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shader.getId(), "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		
		glUniform1f(uniID, 0.5f);
		texture.bind();
		vao.bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vbo.free();
	vao.free();
	ebo.free();
	shader.free();
	texture.free();
	glfwDestroyWindow(window);
	glfwTerminate();
}
