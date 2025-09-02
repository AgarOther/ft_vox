#include <cstdlib>
#include <chrono>
#include "Camera.hpp"
#include "Crosshair.hpp"
#include "Skybox.hpp"
#include "BlockTypeRegistry.hpp"
#include "StructureRegistry.hpp"
#include "ObjectRegistry.hpp"
#include "Player.hpp"
#include "TextureAtlas.hpp"
#include "World.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "imgui/imgui.h"

DebugInfo g_DEBUG_INFO = {0, 0, 0, false};
constexpr unsigned long long WORLD_SEED = 42;

int main(void)
{
	int width, height;
	int fpsGoal = 60;

	if (!glfwInit())
		handleExit(FAILURE_GLFW);

	GLFWwindow * window = initWindow(&width, &height, &fpsGoal);

	#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif

	const ImGuiIO & io = getImGuiIO(window);

	ObjectRegistry::init();
	BlockTypeRegistry::init();
	StructureRegistry::init();
	TextureAtlas atlas;
	atlas.init();

	Shader shader("block.vert", "block.frag");
	Skybox skybox;
	Crosshair crosshair;

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetSeed(WORLD_SEED);

	std::unordered_map<Environment, World * > worlds;
	worlds[OVERWORLD] = new World(&atlas, noise, OVERWORLD);
	worlds[NETHER] = new World(&atlas, noise, NETHER);
	worlds[THE_END] = new World(&atlas, noise, THE_END);

	Player player("Eleonore", width, height, worlds[OVERWORLD]);

	double timeStart, endTime, fpsInterval, sleepTime;
	double deltaTime = io.DeltaTime;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		timeStart = glfwGetTime();
		fpsInterval = 1.0 / fpsGoal;

		const bool hasGui = player.getCamera()->hasGuiOn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, &player, deltaTime, &fpsGoal, worlds);
		g_DEBUG_INFO.drawCalls = 0;

		player.getWorld()->generateProcedurally();
		player.interceptInputs(window, deltaTime);
		player.getCamera()->setupMatrix(shader);
		skybox.render(player.getCamera(), player.getWorld()->getEnvironment());
		player.getWorld()->render(shader, player);
		crosshair.draw(static_cast<float>(player.getCamera()->getWidth()) / static_cast<float>(player.getCamera()->getHeight()));

		if (hasGui)
			renderImGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
		player.getWorld()->applyGravity(deltaTime);

		endTime = glfwGetTime();
		deltaTime = endTime - timeStart;
		sleepTime = fpsInterval - deltaTime;
		if (sleepTime > 0.001)
			USLEEP(static_cast<int>((sleepTime - 0.001) * 1000000));
		while ((glfwGetTime() - timeStart) < fpsInterval)
			;

		deltaTime = glfwGetTime() - timeStart;
	}

	for (auto it = worlds.begin(); it != worlds.end(); ++it)
	{
		it->second->shutdown();
		delete it->second;
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	shutdownImGui();
	return 0;
}
