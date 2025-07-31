#include <cstdlib>
#include "Camera.hpp"
#include "Skybox.hpp"
#include "minecraft/BlockTypeRegistry.hpp"
#include "minecraft/ObjectRegistry.hpp"
#include "minecraft/Player.hpp"
#include "minecraft/TextureAtlas.hpp"
#include "minecraft/World.hpp"
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
		"assets/block/stone.png",
		"assets/block/dirt.png",
		"assets/block/bedrock.png",
		"assets/block/sand.png",
		"assets/block/end_stone.png",
		"assets/block/unknown.png"
	});
	
	Shader shader("block.vert", "block.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");
	Skybox skybox;

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetSeed(WORLD_SEED);

	World world(16, 16, atlas, noise);

	Player player("Eleonore", width, height, &world);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		const bool hasGui = player.getCamera()->hasGuiOn();
		g_DEBUG_INFO.deltaTime = io.DeltaTime;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, player.getCamera());
		g_DEBUG_INFO.drawCalls = 0;
		
		player.getCamera()->setupMatrix(shader);
		player.getCamera()->setWidth(width);
		player.getCamera()->setHeight(height);
		skybox.render(skyboxShader, player.getCamera());
		world.render(shader);
		
		if (hasGui)
			renderImGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
		player.interceptInputs(window, io.DeltaTime);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	shutdownImGui();
	return 0;
}
