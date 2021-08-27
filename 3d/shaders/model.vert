#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 water;

uniform int normaldir = 0;

out vec3 viewDirection;
out vec3 normal;
out vec3 gnormal;
out vec3 worldPosition;
out vec3  worldWater;

void main( void )
{
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    vec4 tworldPosition = modelViewMatrix * vec4(vertexPosition, 1.0);
    gl_Position = projectionMatrix *  tworldPosition;

    viewDirection  = normalize(vec3(-tworldPosition));
    mat3 normalMatrix = mat3(modelViewMatrix);

    normal          = normalMatrix * vertexNormal;
    gnormal        = mat3(modelMatrix) * vertexNormal;
		
	if(normaldir & 1)
	{
		normal.x = -normal.x;
		gnormal.x = -gnormal.x;
	}
	if(normaldir & 2)
	{
		normal.y = -normal.y;
		gnormal.y = -gnormal.y;
	}
	if(normaldir & 4)
	{
		normal.z = -normal.z;
		gnormal.z = -gnormal.z;
	}
		
    worldPosition   = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    worldWater = water;
}
