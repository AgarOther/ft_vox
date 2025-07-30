#include <cstdlib>
#include <iostream>
#include "Camera.hpp"
#include "Skybox.hpp"
#include "minecraft/BlockTypeRegistry.hpp"
#include "minecraft/Chunk.hpp"
#include "minecraft/ObjectRegistry.hpp"
#include "minecraft/TextureAtlas.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "imgui/imgui.h"

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

	const ImGuiIO & io = getImGuiIO(window);

	ObjectRegistry::init();
	BlockTypeRegistry::init();
	TextureAtlas atlas;
	atlas.load({
		"assets/blocks/stone.png",
		"assets/blocks/dirt.png",
		"assets/blocks/bedrock.png",
		"assets/blocks/unknown.png"
	});
	
	Shader shader("block.vert", "block.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, -2.0f));
	Skybox skybox;
	
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
		const bool hasGui = camera.hasGuiOn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, camera);
		skybox.render(skyboxShader, camera);
		
		shader.bind();
		camera.setupMatrix(shader);
		camera.setWidth(width);
		camera.setHeight(height);
		for (Chunk * chunk : chunks)
			chunk->render(shader);
		
		if (hasGui)
			renderImGui();
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
	shutdownImGui();
	return 0;
}
