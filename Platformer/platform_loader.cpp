#include "Headers/platform_loader.h"

#include <fstream>
#include <sstream>


using namespace std;
using namespace glm;

enum Direction {
	UP, LEFT, DOWN, RIGHT
};

void PlatformSet::Load(const GLchar *file) {
	float PosX, PosY, SizeX, SizeY;
	string line;

	this->Plats.clear();
			
	ifstream levelstream(file);
		
	
	if (levelstream) {
		while (getline(levelstream, line)) {
			istringstream sstream(line);

			sstream >> PosX;
			sstream >> PosY;
			sstream >> SizeX;
			sstream >> SizeY;
			
			GameObject obj(glm::vec2(PosX,PosY), glm::vec2(SizeX,SizeY), ResourceManager::GetTexture("platform"));
			
			this->Plats.push_back(obj);
		}
	}
}


void PlatformSet::Draw(SpriteRenderer &renderer, GLuint width, GLuint height) {
	for (GameObject &obj : this->Plats)
	{
		obj.Draw(renderer, width, height);
	}
}

typedef std::tuple<GLboolean, glm::vec2, Direction> Collision;

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

Collision CheckCollision(GameObject &character, GameObject &two) {
	Direction direction;
	vec2 offset = vec2(0, 0);

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
	
	return std::make_tuple(totalCollision, difference, direction);
}


void PlatformSet::Collisions(GameObject &player)
{
	Collision collision;
	glm::vec2 difference;
	Direction direction;


	for (GameObject &obj : this->Plats)
	{
		collision = CheckCollision(player, obj);
		if (std::get<0>(collision)) {

			difference = std::get<1>(collision);
			direction = std::get<2>(collision);

			switch (direction) {
			case LEFT:
				player.Velocity.x = 0.0f;
				player.Position.x -= player.HalfExtent.x - difference.x;
				break;
			case RIGHT:
				player.Velocity.x = 0.0f;
				player.Position.x += player.HalfExtent.x + difference.x;
				break;
			case UP:
				player.Velocity.y = 0.0f;
				player.Position.y -= player.HalfExtent.y - difference.y;
				break;
			case DOWN:
				player.Velocity.y = 0.0f;
				player.Position.y += player.HalfExtent.y + difference.y;
				break;
			}

			break;
		}
	}
}


