#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform vec2 spriteOffset;
uniform vec2 spriteSize;
uniform mat4 model;
uniform mat4 projection;

void main(){
	TexCoords = vertex.zw + spriteOffset;
	gl_Position = projection * model * vec4(spriteSize, 0.0, 1.0);
}