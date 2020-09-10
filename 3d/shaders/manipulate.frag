#version 130

out vec4 fragColor;

uniform vec4 color;

uniform float state;
void main() 
{
	fragColor = color * state;
}
