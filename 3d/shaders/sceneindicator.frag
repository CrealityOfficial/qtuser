#version 330 core

layout (location = 0) out vec4 fragColor;

in vec2 texCoord;
in float dirIndex;
in vec3 normal;

uniform sampler2D colorMap;
uniform int highlightFaces;

void main() 
{
	fragColor = vec4(texture2D(colorMap, texCoord).rgb, 1.0);

	if (highlightFaces == dirIndex) {
		fragColor += 0.3;
	}
}
