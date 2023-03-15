#version 330 core

layout (location = 0) out vec4 fragColor;

in vec2 texCoord;
in float dirIndex;
in vec3 normal;

uniform sampler2D colorMap;
uniform sampler2D selectMap;

uniform float highlightFaces;

void main() 
{	
	float diffuse = dot(normalize(normal), normalize(vec3(1.0, 1.0, 1.0)));
	diffuse = clamp(diffuse, 0.0, 1.0);
	diffuse = mix(0.6, 1.1, diffuse);

	vec4 texColor = texture(colorMap, texCoord);

	if (abs(highlightFaces - dirIndex) < 0.0001) {
		texColor = texture(selectMap, texCoord);
	}

	fragColor = vec4(texColor.rgb * diffuse, 1.0);
}
