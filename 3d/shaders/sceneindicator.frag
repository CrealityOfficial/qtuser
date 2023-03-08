#version 330 core

layout (location = 0) out vec4 fragColor;

in vec2 texCoord;
in float dirIndex;
in vec3 normal;

uniform sampler2D colorMap;
uniform sampler2D selectMap;

uniform int highlightFaces;

void main() 
{	
	float diffuse = dot(normalize(normal), vec3(0.0, 0.0, 1.0));
	diffuse = clamp(diffuse, 0.0, 1.0);
	diffuse = mix(0.6, 1.1, diffuse);

	vec4 texColor = texture2D(colorMap, texCoord);

	if (highlightFaces == dirIndex) {
		texColor = texture2D(selectMap, texCoord);
	}

	fragColor = vec4(texColor.rgb * diffuse, 1.0);
}
