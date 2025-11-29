#include "ObjectRegistry.hpp"
#include "StructureRegistry.hpp"
#include "errors.hpp"
#include "utils.hpp"

Scene::~Scene()
{
	_player->getWorld()->shutdown();
	delete _player;
	delete _camera;
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

	World * startingWorld = WorldManager::createWorld("overworld", OVERWORLD, WORLD_SEED);
	if (!startingWorld)
		handleExit(FAILURE_WORLD_CREATION);
	WorldManager::createWorld("nether", NETHER, WORLD_SEED);
	WorldManager::createWorld("the_end", THE_END, WORLD_SEED);

	_camera = new Camera(_width, _height, Location(0.0, CAMERA_OFFSET_Y, 0.0).getVec3());
	_player = new Player("Eleonore", _camera, startingWorld);

	#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	#endif
}
