#include <cstdlib>
#include <chrono>
#include "BlockOverlay.hpp"
#include "imgui/imgui.h"
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
#include "Filter.hpp"
#include "Scene.hpp"
#include "InputManager.hpp"

DebugInfo g_DEBUG_INFO = {0, 0, 0, false};
constexpr Seed WORLD_SEED = 42;

int main(void)
{
	Scene scene;

	scene.init();

	const ImGuiIO & io = getImGuiIO(scene.getWindow());

	Shader shader("block.vert", "block.frag");
	Skybox skybox;
	Crosshair crosshair;
	BlockOverlay blockOverlay;
	Filter filter;

	Player * player = scene.getPlayer();
	Camera * camera = scene.getCamera();

	double timeStart, endTime, fpsInterval, sleepTime;
	double deltaTime = io.DeltaTime;

	// Main loop
	while (!glfwWindowShouldClose(scene.getWindow()))
	{
		glfwPollEvents();
		InputManager::interceptMouse(&scene);
		InputManager::interceptKeyboard(&scene, deltaTime);
		timeStart = glfwGetTime();
		fpsInterval = 1.0 / scene.getFPSGoal();

		const bool hasGui = camera->hasGuiOn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, deltaTime, &scene);
		g_DEBUG_INFO.drawCalls = 0;

		player->getWorld()->applyGravity(deltaTime);
		camera->setupMatrix(shader);
		skybox.render(camera, player->getWorld()->getEnvironment());
		player->getWorld()->render(shader);
		if (!scene.isPureViewModeEnabled())
		{
			crosshair.draw(static_cast<float>(camera->getWidth()) / static_cast<float>(camera->getHeight()));
			blockOverlay.draw(player, deltaTime);
		}
		if (player->getWorld()->getBlockAt(player->getEyeLocation()).material == WATER)
			filter.draw(glm::vec4(0.1f, 0.0f, 0.95f, 0.2f));

		if (hasGui)
			renderImGui();
		glfwSwapBuffers(scene.getWindow());

		endTime = glfwGetTime();
		deltaTime = endTime - timeStart;
		sleepTime = fpsInterval - deltaTime;
		if (sleepTime > 0)
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));

		deltaTime = glfwGetTime() - timeStart;
	}

	return 0;
}
