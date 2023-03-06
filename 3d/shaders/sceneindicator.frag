#version 330 core

layout (location = 0) out vec4 fragColor;

in vec2 texCoord;
in float dirIndex;
in vec3 normal;

uniform sampler2D colorMap;
uniform int highlightFaces;

void main() 
{
	
	float diffuse = dot(normalize(normal), vec3(0.0, 0.0, 1.0));
	diffuse = clamp(diffuse, 0.0, 1.0);
	diffuse = mix(0.6, 1.1, diffuse);

	fragColor = vec4(texture2D(colorMap, texCoord).rgb, 1.0) * diffuse;

	fragColor += (highlightFaces == dirIndex) ? 0.2: 0.0;
	// if (highlightFaces == dirIndex) {
	// 	fragColor += 0.2;
	// }
}
