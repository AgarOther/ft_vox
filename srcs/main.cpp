#include <cstdlib>
#include <iostream>
#include "Camera.hpp"
#include "Shader.hpp"
#include "minecraft/BlockTypeRegistry.hpp"
#include "minecraft/Chunk.hpp"
#include "minecraft/ObjectRegistry.hpp"
#include "minecraft/TextureAtlas.hpp"
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
	TextureAtlas atlas;
	atlas.load({
		"assets/blocks/stone.png",
		"assets/blocks/unknown.png"
	});
	
	Shader shader("test.vert", "test.frag");
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, -2.0f));
	
	std::vector<Chunk * > chunks;
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			Chunk * chunk = new Chunk(x, z);
			chunk->generateMesh(atlas);
			chunks.push_back(chunk);
		}
	}

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
		for (Chunk * chunk : chunks)
			chunk->render(shader);
		
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
	for (Chunk * chunk : chunks)
		delete chunk;
	return 0;
}
