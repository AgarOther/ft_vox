#include <cstdlib>
#include <chrono>
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
#include "Scene.hpp"

DebugInfo g_DEBUG_INFO = {0, 0, 0, false};
constexpr unsigned long long WORLD_SEED = 42;

int main(void)
{
	Scene scene;

	scene.init();

	const ImGuiIO & io = getImGuiIO(scene.getWindow());

	Shader shader("block.vert", "block.frag");
	Skybox skybox;
	Crosshair crosshair;

	Player * player = scene.getPlayer();

	double timeStart, endTime, fpsInterval, sleepTime;
	double deltaTime = io.DeltaTime;

	// Main loop
	while (!glfwWindowShouldClose(scene.getWindow()))
	{
		timeStart = glfwGetTime();
		fpsInterval = 1.0 / scene.getFPSGoal();

		const bool hasGui = player->getCamera()->hasGuiOn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (hasGui)
			showImGui(io, player, deltaTime, &scene);
		g_DEBUG_INFO.drawCalls = 0;

		player->getWorld()->generateProcedurally();
		player->interceptInputs(scene.getWindow(), deltaTime);
		player->getCamera()->setupMatrix(shader);
		skybox.render(player->getCamera(), player->getWorld()->getEnvironment());
		player->getWorld()->render(shader, *player);
		crosshair.draw(static_cast<float>(player->getCamera()->getWidth()) / static_cast<float>(player->getCamera()->getHeight()));

		if (hasGui)
			renderImGui();
		glfwSwapBuffers(scene.getWindow());
		glfwPollEvents();
		player->getWorld()->applyGravity(deltaTime);

		endTime = glfwGetTime();
		deltaTime = endTime - timeStart;
		sleepTime = fpsInterval - deltaTime;
		if (sleepTime > 0.001)
			USLEEP(static_cast<int>((sleepTime - 0.001) * 1000000));
		while ((glfwGetTime() - timeStart) < fpsInterval)
			;

		deltaTime = glfwGetTime() - timeStart;
	}

	return 0;
}
