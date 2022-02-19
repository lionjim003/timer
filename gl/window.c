#include "depend.h"
#include "log.h"

static void 
glfw_error (int error, const char *message) {
	LOG ("GLFW", "error %#010x: %s\n", error, message);
}

GLFWwindow *window_context (int width, int height, char *title, GLFWmonitor *monitor, GLFWwindow *share) {
	glfwSetErrorCallback (glfw_error);
	if (!glfwInit ())
		return NULL;
	
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow (width, height, title, monitor, share);
	if (!window) {
		LOG ("GLFW", "Failed to create Window or OpenGL context\n");
		return NULL;
	}
	
	glfwMakeContextCurrent (window);
	if (!gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress)) {
		LOG ("GLAD", "Failed to initilize OpenGL context\n");
		return NULL;
	}
	return window;
}

void 
framebuffer_size_callback (GLFWwindow *window, int width, int height) {
	glViewport (0, 0, width, height);
}

void
window_callback (GLFWwindow *window) {
	glfwSetFramebufferSizeCallback (window, framebuffer_size_callback);
}

typedef void (GLFWrender (GLFWwindow *, float, void *));

void
window_render (GLFWwindow *window, void *args, GLFWrender *render_function) {
	
	float deltaTime, currentFrame, lastFrame = 0.0f;
	glfwSwapInterval (1);
	
	while (!glfwWindowShouldClose (window)) {
		if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose (window, 1);
		
		currentFrame = (float) glfwGetTime ();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		render_function (window, deltaTime, args);
		glfwSwapBuffers (window);
		glfwPollEvents ();
	}
	glfwDestroyWindow (window);
	glfwTerminate ();
}