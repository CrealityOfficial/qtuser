#version 330 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec3 worldPosion;
layout (location = 2) out vec3 worldNormal;

in vec2 texCoord;
in vec3 normal;
in vec4 encodeIndex;

void main() 
{
	fragColor = encodeIndex;
}
