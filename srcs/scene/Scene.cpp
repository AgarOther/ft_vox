#include "ObjectRegistry.hpp"
#include "StructureRegistry.hpp"
#include "errors.hpp"
#include "utils.hpp"

Scene::~Scene()
{
	delete _player;
	TextureAtlas::destroy();
	glfwDestroyWindow(_window);
	glfwTerminate();
	shutdownImGui();
}

void Scene::init()
{
	if (!glfwInit())
		handleExit(FAILURE_GLFW);

	_window = initWindow(&_width, &_height, &_fpsGoal);
	glfwSetWindowUserPointer(_window, this);

	ObjectRegistry::init();
	BlockTypeRegistry::init();
	StructureRegistry::init();
	TextureAtlas::init();

	WorldManager::createWorld("overworld", OVERWORLD, WORLD_SEED);
	WorldManager::createWorld("overworld", NETHER, WORLD_SEED);
	WorldManager::createWorld("overworld", THE_END, WORLD_SEED);

	_camera = new Camera(_width, _height, Location(0.0, CAMERA_OFFSET_Y, 0.0).getVec3());
	_player = new Player("Eleonore", _camera, WorldManager::getWorld("overworld"));

	#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif
}
