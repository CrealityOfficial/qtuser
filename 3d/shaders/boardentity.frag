#version 150 core

out vec4 fragColor;

uniform sampler2D boardTexture;

in vec2 texcoord;
void main() 
{
	vec4 color = texture(boardTexture, texcoord);
	fragColor = color;  
}
