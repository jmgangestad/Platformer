
#include <tuple>
#include <ctime>
#include "Headers/game.h"
#include "Headers/resource_manager.h"
#include "Headers/sprite_renderer.h"
#include "Headers/game_object.h"
#include <cstdlib>
#include <irrklang/irrKlang.h>
#include <vector>


#include <iostream>
#include <sstream>
#include <fstream>



using namespace irrklang;
using namespace glm;

SpriteRenderer *Renderer;
GameObject *Player;
GameObject *Platform;




ISoundEngine *SoundEngine = createIrrKlangDevice();



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
	delete Player;
}

void Game::Init() {
	/* ALWAYS LEAVE THESE */
	srand(time(NULL));
	ResourceManager::LoadShader("Data/shaders/sprite.vs", "Data/shaders/sprite.frag", 0, "sprite");
	ResourceManager::LoadShader("Data/shaders/particle.vs", "Data/shaders/particle.frag", 0, "particle");
	mat4 projection = ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	/* ALWAYS LEAVE THESE */

	/* TEXTURES GO HERE */
    //      example  LoadTexture("Data/Resources/textures/image.jpg", USE_ALPHA, "name");
	ResourceManager::LoadTexture("Data/Resources/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("Data/Resources/textures/block.png", GL_TRUE, "player");
	ResourceManager::LoadTexture("Data/Resources/textures/block.png", GL_TRUE, "platform");
	/* TEXTURES GO HERE */


	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	

	vec2 playerPos = vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height/2 - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));
	Player->HasGravity = true;
	Player->Gravity = 700.0f;
	

	vec2 platformSize = vec2(this->Width/2, 10);
	vec2 platformPos = vec2(this->Width / 2 - platformSize.x / 2, 3*this->Height / 4 - platformSize.y/2);
	Platform = new GameObject(platformPos, platformSize, ResourceManager::GetTexture("platform"));
	Platform->HasGravity = false;
	Platform->Gravity = 1.0f;

	/* Many GameObjects example
	for (int i = 0; i < num_asteroids; i++)
	{
		AsteroidObject obj(vec2(rand() % this->Width, rand() % this->Height), 15 + rand() % 40, vec2(-50.0f + rand() % 100, -50.0f + rand() % 100), ResourceManager::GetTexture("asteroid"));
		obj.AngularRate = -50.0f + rand() % 100;
		this->Asteroids.push_back(obj);
	}
	*/

	
	// SoundEngine->play2D("Data/Resources/audio/background.mp3", GL_TRUE);
}

void Game::Update(GLfloat dt) {

	Player->Move(dt, this->Width, this->Height);
	Platform->Move(dt, this->Width, this->Height);

	/* Many GameObjects example
	for (AsteroidObject &tile : this->Asteroids)
	{
		tile.Move(dt, this->Width, this->Height);
	}
	*/
	

	this->DoCollisions();
}

void Game::Render() {
	if (this->State == GAME_ACTIVE) {

		Renderer->DrawSprite(ResourceManager::GetTexture("background"), vec2(0, 0), vec2(this->Width, this->Height), 0.0f);
		
		Player->Draw(*Renderer, this->Width, this->Height);
		Platform->Draw(*Renderer, this->Width, this->Height);
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

			Player->Velocity.y -= 300.0f;



			SoundEngine->play2D("Data/Resources/audio/bleep.wav", GL_FALSE);

			last_button_a = true;
		}
		else if (!button[a])
		{
			last_button_a = false;
		}

		if (button[y] && !last_button_y)
		{
			// button pressed but not held

			Player->HasGravity = !Player->HasGravity;
			Player->Velocity = vec2(0.0f, 0.0f);

			
			SoundEngine->play2D("Data/Resources/audio/plink.wav", GL_FALSE);

			last_button_y = true;
		}
		else if (!button[y])
		{
			last_button_y = false;
		}

		if (axes[left_joy_x] != 0) {
			Player->Position.x += 2.0f*axes[left_joy_x];
		}
		
		if (axes[left_joy_y] != 0) {
			//Player->Position.y -= 2.0f*axes[left_joy_y];
		}

		float dpadspeed = 1.5f;

		if (button[Dleft]) {
			Player->Position.x -= dpadspeed;
		}
		if (button[Dright]) {
			Player->Position.x += dpadspeed;
		}

		if (button[Dup]) {
			Player->Position.y -= dpadspeed;
		}

		if (button[Ddown]) {
			Player->Position.y += dpadspeed;
		}
	}
	/* CONTROLLER INPUT END */

	/* KEYBOARD INPUT BEGIN */
	if (this->Keys[GLFW_KEY_A]) {
		
	}
	if (this->Keys[GLFW_KEY_D]) {
		
	}

	if (this->Keys[GLFW_KEY_W]) {
		
	}

	if (this->Keys[GLFW_KEY_S]) {
		
	}
	/* KEYBOARD INPUT END */
}





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



typedef std::tuple<GLboolean, GLboolean, GLboolean, vec2, Direction> Collision;

Collision CheckCollision(GameObject &character, GameObject &two) {
	Direction direction;
	vec2 offset = vec2(0,0);
	
	bool collisionX = character.Position.x + character.Size.x >= two.Position.x && two.Position.x + two.Size.x >= character.Position.x;
	bool collisionY = character.Position.y + character.Size.y >= two.Position.y && two.Position.y + two.Size.y >= character.Position.y;

	bool totalCollision = collisionX && collisionY;
	
	character.UpdateCenter();
	two.UpdateCenter();
	
	
	vec2 difference = character.Center - two.Center;
	vec2 clamped = clamp(difference, -two.HalfExtent, two.HalfExtent);

	vec2 closest = two.Center + clamped;

	difference = closest - character.Center;

	direction = VectorDirection(difference);
	
	offset = character.HalfExtent - difference;

	std::cout << character.HalfExtent.x - difference.x << std::endl;
	


	return std::make_tuple(totalCollision, collisionX, collisionY, difference, direction);
}



void Game::DoCollisions() {
	// Collision checks go here

	Collision collision = CheckCollision(*Player, *Platform);

	GLboolean totalCollision = std::get<0>(collision);
	GLboolean collisionX = std::get<1>(collision);
	GLboolean collisionY = std::get<2>(collision);
	vec2 difference = std::get<3>(collision);
	Direction direction = std::get<4>(collision);



	if (totalCollision) {
		switch (direction) {
		case LEFT:
			Player->Velocity.x = 0.0f;
			Player->Position.x -= Player->HalfExtent.x - difference.x;
			break;
		case RIGHT:
			Player->Velocity.x = 0.0f;
			Player->Position.x += Player->HalfExtent.x + difference.x;
			break;
		case UP:
			Player->Velocity.y = 0.0f;
			Player->Position.y -= Player->HalfExtent.y - difference.y;
			break;
		case DOWN:
			Player->Velocity.y = 0.0f;
			Player->Position.y += Player->HalfExtent.y + difference.y;
			break;



		}
	}
	

}