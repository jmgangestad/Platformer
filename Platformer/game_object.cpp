#include "Headers/game_object.h"


GameObject::GameObject()
	:Position(0, 0), Size(1, 1), Velocity(0.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false), AngularRate(0.0f), Gravity(1.0f), HasGravity(false), Depth(0.0f)
{
	this->HalfExtent = this->Size / 2.0f;
	this->Center = this->Position + this->HalfExtent;
}

GameObject::GameObject(glm::vec2 pos, GLfloat depth, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), AngularRate(0.0f), Gravity(1.0f), HasGravity(false), Depth(depth)
{
	this->HalfExtent = this->Size / 2.0f;
	this->Center = this->Position + this->HalfExtent;
}

void GameObject::UpdateCenter()
{
	this->Center = this->Position + this->HalfExtent;
}

void GameObject::Draw(SpriteRenderer &renderer, GLuint window_width, GLuint window_height) {
	
	renderer.DrawSprite(this->Sprite, this->Position, this->Depth, this->Size, this->Rotation);
	
	/* USE THESE IF YOU WANT TO CONTINUE FROM ONE SIDE OF SCREEN TO THE OTHER*/
	if (this->Position.x + this->Size.x >= window_width)
	{
		renderer.DrawSprite(this->Sprite, glm::vec2(this->Position.x - window_width, this->Position.y), this->Depth, this->Size, this->Rotation);
	}

	if (this->Position.y + this->Size.y >= window_height)
	{
		renderer.DrawSprite(this->Sprite, glm::vec2(this->Position.x, this->Position.y - window_height), this->Depth, this->Size, this->Rotation);
	}

	if (this->Position.x + this->Size.x >= window_width && this->Position.y + this->Size.y >= window_height)
	{
		renderer.DrawSprite(this->Sprite, glm::vec2(this->Position.x - window_width, this->Position.y - window_height), this->Depth, this->Size, this->Rotation);
	}
	/* USE THESE IF YOU WANT TO CONTINUE FROM ONE SIDE OF SCREEN TO THE OTHER*/
}



glm::vec2 GameObject::Move(GLfloat dt, GLuint window_width, GLuint window_height)
{
	if (this->HasGravity)
	{
		this->Velocity.y += this->Gravity * dt;
	}
	this->Position += this->Velocity * dt;
	this->Rotation += this->AngularRate * dt;


	/* USE THESE IF YOU WANT TO CONTINUE FROM ONE SIDE OF SCREEN TO THE OTHER*/
	if (this->Position.x < 0.0f)
	{
		this->Position.x = window_width;
	}
	else if (this->Position.x > window_width)
	{
		this->Position.x = 0.0f;
	}

	if (this->Position.y < 0.0f)
	{
		this->Position.y = window_height;
	}
	else if (this->Position.y > window_height)
	{
		this->Position.y = 0.0f;
	}
	/* USE THESE IF YOU WANT TO CONTINUE FROM ONE SIDE OF SCREEN TO THE OTHER*/

	return this->Position;
}