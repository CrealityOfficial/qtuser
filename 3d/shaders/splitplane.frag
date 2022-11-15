#version 150 core

out vec4 fragColor;

in vec3 worldPosition;
uniform float borderWidth;
uniform vec4 color;
uniform vec3 clip[2];

void main() 
{
	if(worldPosition.z < clip[0].z || worldPosition.z > clip[1].z
		|| worldPosition.x < clip[0].x || worldPosition.x > clip[1].x
		|| worldPosition.y < clip[0].y || worldPosition.y > clip[1].y)
		discard;
		
	fragColor = color;

	if (worldPosition.x < clip[0].x + borderWidth || worldPosition.x > clip[1].x - borderWidth || worldPosition.y < clip[0].y + borderWidth || worldPosition.y > clip[1].y - borderWidth)
		fragColor.w = 1.0;
}
