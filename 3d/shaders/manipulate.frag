#version 150 core

out vec4 fragColor;

uniform vec4 color;
uniform vec4 changecolor;
uniform int mt;

uniform float state;
void main() 
{
	if(mt == 0)
	{
		fragColor = color * state;
	}
	else if(mt == 1)
	{
		fragColor = color + changecolor * state;
	}
	
}
