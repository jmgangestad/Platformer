#ifndef GAME_H
#define GAME_H
#include <vector>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include "../../Platformer/Headers/game_object.h"
#include "../../Platformer/Headers/platform_loader.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP, LEFT, DOWN, RIGHT
};

const glm::vec2 PLAYER_SIZE(45, 54);

const GLfloat PLAYER_VELOCITY(500.0f);


class GameBuilder {
public:
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;
	bool last_button_a = false;
	bool last_button_y = false;
	//std::vector<GameObject> Platforms;

	GameBuilder(GLuint width, GLuint height);
	~GameBuilder();

	void Init();
	void ProcessInput(GLfloat dt, GLFWwindow *window);
	void Update(GLfloat dt);
	void Render();
	void DoCollisions();
};


#endif