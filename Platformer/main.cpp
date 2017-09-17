#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "Headers/game.h"
#include "Headers/resource_manager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

Game VideoGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[]) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Basic Blank Game", 0, 0);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VideoGame.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;


	VideoGame.State = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		VideoGame.ProcessInput(deltaTime, window);

		VideoGame.Update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		VideoGame.Render();

		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, GL_TRUE); }

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			VideoGame.Keys[key] = GL_TRUE;
		} else if (action == GLFW_RELEASE) {
			VideoGame.Keys[key] = GL_FALSE;
		}
	}
}
