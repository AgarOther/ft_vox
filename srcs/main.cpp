#include <cstdlib>
#include "Camera.hpp"
#include "Crosshair.hpp"
#include "Skybox.hpp"
#include "minecraft/BlockTypeRegistry.hpp"
#include "minecraft/ObjectRegistry.hpp"
#include "minecraft/Player.hpp"
#include "minecraft/TextureAtlas.hpp"
#include "minecraft/World.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "imgui/imgui.h"


DebugInfo g_DEBUG_INFO = {0, 0, 0, false};
const unsigned long long WORLD_SEED = 42;

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
	atlas.init();

	Shader shader("block.vert", "block.frag");
	Skybox skybox;
	Crosshair crosshair;

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetSeed(WORLD_SEED);

	World world(32, 32, atlas, noise);

	Player player("Eleonore", width, height, &world);

	double timeStart = glfwGetTime();
	double endTime;
	double deltaTime = 0;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		const bool hasGui = player.getCamera()->hasGuiOn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, &player, deltaTime);
		g_DEBUG_INFO.drawCalls = 0;

		player.interceptInputs(window, deltaTime);
		player.getCamera()->setupMatrix(shader);
		skybox.render(player.getCamera());
		shader.bind();
		world.render(shader, player);
		crosshair.draw();

		if (hasGui)
			renderImGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
		endTime = glfwGetTime();
		deltaTime = endTime - timeStart;
		world.applyGravity(deltaTime);
		timeStart = glfwGetTime();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	shutdownImGui();
	return 0;
}
