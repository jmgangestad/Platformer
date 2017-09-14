
#include <tuple>
#include <ctime>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include <cstdlib>
#include <irrklang/irrKlang.h>
#include <vector>

using namespace irrklang;

SpriteRenderer *Renderer;
GameObject *Ship;


//typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

ISoundEngine *SoundEngine = createIrrKlangDevice();

/*
Direction VectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++) {
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}
*/

/*
GLboolean CheckCollision(GameObject &one, GameObject &two) {
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
	return collisionX && collisionY;
}
*/

enum {
	a, b, x, y, lb, rb, back, start, lclick, rclick, Dup, Dright, Ddown, Dleft
};

enum {
	left_joy_x, left_joy_y, right_joy_x, right_joy_y, L_trigger, R_trigger
};

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height){

}

Game::~Game() {
	delete Renderer;
	delete Ship;
}

void Game::Init() {
	/* ALWAYS LEAVE THESE */
	srand(time(NULL));
	ResourceManager::LoadShader("Data/shaders/sprite.vs", "Data/shaders/sprite.frag", 0, "sprite");
	ResourceManager::LoadShader("Data/shaders/particle.vs", "Data/shaders/particle.frag", 0, "particle");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	/* ALWAYS LEAVE THESE */

	/* TEXTURES GO HERE */
    //      example  LoadTexture("Data/Resources/textures/image.jpg", USE_ALPHA, "name");
	ResourceManager::LoadTexture("Data/Resources/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("Data/Resources/textures/ship.png", GL_TRUE, "ship");
	ResourceManager::LoadTexture("Data/Resources/textures/block.png", GL_TRUE, "block");
	/* TEXTURES GO HERE */


	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	

	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height/2 - PLAYER_SIZE.y);
	Ship = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("ship"));
	

	/* Many GameObjects example
	for (int i = 0; i < num_asteroids; i++)
	{
		AsteroidObject obj(glm::vec2(rand() % this->Width, rand() % this->Height), 15 + rand() % 40, glm::vec2(-50.0f + rand() % 100, -50.0f + rand() % 100), ResourceManager::GetTexture("asteroid"));
		obj.AngularRate = -50.0f + rand() % 100;
		this->Asteroids.push_back(obj);
	}
	*/

	
	SoundEngine->play2D("Data/Resources/audio/background.mp3", GL_TRUE);
}

void Game::Update(GLfloat dt) {

	Ship->Move(dt, this->Width, this->Height);
	
	/* Many GameObjects example
	for (AsteroidObject &tile : this->Asteroids)
	{
		tile.Move(dt, this->Width, this->Height);
	}
	*/
	

	//this->DoCollisions();
}

void Game::Render() {
	if (this->State == GAME_ACTIVE) {

		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		
		Ship->Draw(*Renderer, this->Width, this->Height);

		/* Many GameObjects example
		for (AsteroidObject &item : this->Asteroids)
		{
			item.Draw(*Renderer, this->Width, this->Height);
		}
		*/
	}
}

void Game::ProcessInput(GLfloat dt, GLFWwindow *window) {
	int count;


	/* CONTROLLER INPUT BEGIN */
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		const unsigned char *button = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

		if (button[back]) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (button[a] && !last_button_a)
		{
			// button pressed but not held
			SoundEngine->play2D("Data/Resources/audio/bleep.wav", GL_FALSE);

			last_button_a = true;
		}
		else if (!button[a])
		{
			last_button_a = false;
		}

		if (axes[left_joy_x] != 0) {
			Ship->Rotation += 1.2 * axes[left_joy_x];
		}
		
		if (axes[left_joy_y] != 0) {
			Ship->Velocity += glm::vec2(1.2*axes[left_joy_y] * sin(glm::radians(Ship->Rotation)), -1.2*axes[left_joy_y] * cos(glm::radians(Ship->Rotation)));
		}

		if (button[Dleft]) {
			Ship->Rotation -= 1.2f;
		}
		if (button[Dright]) {
			Ship->Rotation += 1.2f;
		}

		if (button[Dup]) {
			Ship->Velocity += glm::vec2(1.2*sin(glm::radians(Ship->Rotation)), -1.2*cos(glm::radians(Ship->Rotation)));
		}

		if (button[Ddown]) {
			Ship->Velocity -= glm::vec2(1.2*sin(glm::radians(Ship->Rotation)), -1.2*cos(glm::radians(Ship->Rotation)));
		}
	}
	/* CONTROLLER INPUT END */

	/* KEYBOARD INPUT BEGIN */
	if (this->Keys[GLFW_KEY_A]) {
		Ship->Rotation -= 1.2f;
	}
	if (this->Keys[GLFW_KEY_D]) {
		Ship->Rotation += 1.2f;
	}

	if (this->Keys[GLFW_KEY_W]) {
		Ship->Velocity += glm::vec2(1.2*sin(glm::radians(Ship->Rotation)), -1.2*cos(glm::radians(Ship->Rotation)));
	}

	if (this->Keys[GLFW_KEY_S]) {
		Ship->Velocity -= glm::vec2(1.2*sin(glm::radians(Ship->Rotation)), -1.2*cos(glm::radians(Ship->Rotation)));
	}
	/* KEYBOARD INPUT END */
}


void Game::DoCollisions() {
	// Collision checks go here
}