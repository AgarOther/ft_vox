#include <cstdlib>
#include "Camera.hpp"
#include "Skybox.hpp"
#include "minecraft/BlockTypeRegistry.hpp"
#include "minecraft/Chunk.hpp"
#include "minecraft/ObjectRegistry.hpp"
#include "minecraft/Player.hpp"
#include "minecraft/TextureAtlas.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "imgui/imgui.h"

DebugInfo g_DEBUG_INFO = {0, 0, 0, 0};
const unsigned long long WORLD_SEED = 420;

int main(void)
{
	int width, height;

	if (!glfwInit())
		handleExit(1, FAILURE_GLFW);

	GLFWwindow * window = initWindow(&width, &height);

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
	Camera camera(width, height, glm::vec3(32.0f, 66.0f, 32.0f));
	Skybox skybox;

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetSeed(WORLD_SEED);
	
	std::vector<Chunk * > chunks;
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			Chunk * chunk = new Chunk(x, z, noise);
			chunk->generateMesh(atlas);
			chunks.push_back(chunk);
		}
	}

	Player player("Eleonore", camera);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		const bool hasGui = camera.hasGuiOn();
		g_DEBUG_INFO.deltaTime = io.DeltaTime;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, camera);
		g_DEBUG_INFO.drawCalls = 0;
		
		camera.setupMatrix(shader);
		camera.setWidth(width);
		camera.setHeight(height);
		skybox.render(skyboxShader, camera);
		shader.bind();
		for (Chunk * chunk : chunks)
			chunk->render(shader);
		
		if (hasGui)
			renderImGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.interceptInputs(window, io.DeltaTime);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	for (Chunk * chunk : chunks)
		delete chunk;
	shutdownImGui();
	return 0;
}
