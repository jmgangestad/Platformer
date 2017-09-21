#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform vec2 spriteOffset;
uniform vec2 spriteSize;
uniform mat4 model;
uniform mat4 projection;

void main(){
	TexCoords = vec2(spriteOffset.x + (vertex.z*spriteSize.x), spriteOffset.y + (vertex.w*spriteSize.y));
	//TexCoords = vec2((vertex.z + spriteOffset.x)*spriteSize.x, (vertex.w + spriteOffset.y)*spriteSize.y);
	//gl_Position = projection * model * vec4(vertex.xy + spriteSize, 0.0, 1.0);
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}