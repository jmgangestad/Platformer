#include "Headers/sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader) {
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, GLfloat depth, glm::vec2 size, GLfloat rotate, glm::vec3 color) {

	this->shader.Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, depth));

	model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f*size.x, -0.5f*size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);

	this->shader.SetVector2f("spriteOffset", glm::vec2(0.0f, 0.0f));
	this->shader.SetVector2f("spriteSize", glm::vec2(1.0f, 1.0f));

	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f,  0.0f, 1.0f,
		1.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f,  0.0f, 0.0f,

		0.0f, 1.0f,  0.0f, 1.0f,
		1.0f, 1.0f,  1.0f, 1.0f,
		1.0f, 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSpriteSheet(int movestate, Texture2D &texture, glm::vec2 position, GLfloat depth, GLint framesPerRow, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	static int direction = 1;
	static int i = 0;
	static int counter = 0;
	float offsetX = 0.0f;
	float offsetY = 0.0f;

	float sheetWidth = 900.0f;
	float sheetHeight = 495.0f;
	framesPerRow = 5.0f;
	float framesPerColumn = 2.0f;

	float sizeX = 1.0f/framesPerRow;
	float sizeY = 1.0f/framesPerColumn;

	if (movestate != 0)
	{
		if (counter == 15)
		{
			i++;
			if (i > framesPerRow-1)
			{
				i = 0;
			}
			counter = 0;
		}
		counter++;
	}
	else
	{
		i = 4;
	}


	offsetX = sizeX * i;
	offsetY = sizeY * 0;

	this->shader.Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, depth));

	model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f*size.x, -0.5f*size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	

	this->shader.SetMatrix4("model", model);

	

	if (movestate == -1)
	{
		direction = -1;
	}
	else if (movestate == 1)
	{
		direction = 1;
	}


	this->shader.SetVector2f("spriteOffset", glm::vec2(offsetX, offsetY));
	this->shader.SetVector2f("spriteSize", glm::vec2(direction*sizeX, sizeY));

	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


}

