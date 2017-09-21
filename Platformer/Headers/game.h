#ifndef GAME_H
#define GAME_H
#include <vector>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include "game_object.h"
#include "platform_loader.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP, LEFT, DOWN, RIGHT
};

const glm::vec2 PLAYER_SIZE(30, 36);

const GLfloat PLAYER_VELOCITY(500.0f);


class Game {
public:
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;
	bool last_button_a = false;
	bool last_button_y = false;
	//std::vector<GameObject> Platforms;

	Game(GLuint width, GLuint height);
	~Game();

	void Init();
	void ProcessInput(GLfloat dt, GLFWwindow *window);
	void Update(GLfloat dt);
	void Render();
	void DoCollisions();
};


#endif