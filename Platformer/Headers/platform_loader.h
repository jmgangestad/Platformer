#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#define GLEW_STATIC
#include <glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


class PlatformSet {
public:
	std::vector<GameObject> Plats;

	PlatformSet() {}
	void Load(const GLchar *file);
	void Draw(SpriteRenderer &renderer, GLuint width, GLuint height);
	void Collisions(GameObject &player);
	//void Draw(SpriteRenderer &renderer);


private:
	//void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif
