#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#define GLEW_STATIC
#include <glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


class GameObject {
public:
	glm::vec2 Position, Size, Velocity;
	GLfloat Rotation;
	GLfloat AngularRate;
	GLboolean IsSolid;
	GLboolean Destroyed;

	Texture2D Sprite;

	glm::vec2 Move(GLfloat dt, GLuint window_width, GLuint window_height);

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(SpriteRenderer &renderer, GLuint window_width, GLuint window_height);
};


#endif