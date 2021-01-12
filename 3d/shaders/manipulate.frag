#version 150 core

out vec4 fragColor;

uniform vec4 color;

uniform float state;
void main() 
{
	fragColor = color * state;
}
