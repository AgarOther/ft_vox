#include "errors.hpp"
#include "utils.hpp"

void toggleFullscreen(GLFWwindow * window, Camera * camera)
{
	static int windowPosX = 0;
	static int windowPosY = 0;
	static int windowWidth = 0;
	static int windowHeight = 0;

	camera->setFullscreen(!camera->isFullscreen());
	if (camera->isFullscreen())
	{
		glfwGetWindowPos(window, &windowPosX, &windowPosY);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		camera->setHeight(mode->height);
		camera->setWidth(mode->width);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, windowPosX, windowPosY, windowWidth, windowHeight, 0);
		camera->setHeight(windowHeight);
		camera->setWidth(windowWidth);
	}
}

static void frameBufferCallback(GLFWwindow * window, int width, int height)
{
	(void) window;
	int viewportWidth, viewportHeight, viewportX, viewportY;
	float targetRatio = 16.0f / 9.0f;
	float windowRatio = static_cast<float>(width) / static_cast<float>(height);

	if (windowRatio > targetRatio)
	{
		viewportHeight = height;
		viewportWidth = static_cast<int>(height * targetRatio);
		viewportX = (width - viewportWidth) / 2;
		viewportY = 0;
	}
	else
	{
		viewportWidth = width;
		viewportHeight = static_cast<int>(width / targetRatio);
		viewportX = 0;
		viewportY = (height - viewportHeight) / 2;
	}
	glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

GLFWwindow * initWindow(int *width, int *height)
{
	GLFWwindow * window;
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(mode->width, mode->height, "player", nullptr, nullptr);
	if (!window)
		handleExit(FAILURE_WINDOW);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		handleExit(FAILURE_GLEW);
	glfwGetFramebufferSize(window, width, height);
	glViewport(0, 0, *width, *height);
	glfwSetFramebufferSizeCallback(window, frameBufferCallback);
	glfwSwapInterval(0); // unlimited (0) | vsync(1)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// just clearing window so it doesn't look weird on start
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);
	return window;
}
