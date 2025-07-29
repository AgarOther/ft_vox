#include <cstdlib>
#include <iostream>
#include "Camera.hpp"
#include "Shader.hpp"
#include "minecraft/BlockTypeRegistry.hpp"
#include "minecraft/Chunk.hpp"
#include "minecraft/ObjectRegistry.hpp"
#include "utils.hpp"
#include "errors.hpp"

int main(void)
{
	int width, height;

	if (!glfwInit())
		handleExit(1, FAILURE_GLFW);

	GLFWwindow *window = initWindow(&width, &height);

	#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif

	ObjectRegistry::init();
	BlockTypeRegistry::init();
	Chunk chunk(0, 0);
	chunk.generateMesh();

	Shader shader("test.vert", "test.frag");
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, -2.0f));

	// FPS
	double start = glfwGetTime();
	double end;
	unsigned int frames = 0;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.bind();
		camera.setWidth(width);
		camera.setHeight(height);
		camera.setupMatrix(shader);
		chunk.render(shader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		// FPS
		end = glfwGetTime();
		camera.interceptInputs(window);
		frames++;
		if (end - start >= 1.0)
		{
			start = end;
			std::cout << "\r" << frames << " FPS" << std::flush;
			frames = 0;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
