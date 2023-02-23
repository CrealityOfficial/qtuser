#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 stepsFlag;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 normal;
flat out vec2 step;
out vec3 viewDirection;

void main( void )
{	
	mat4 modelview_matrix = viewMatrix * modelMatrix;
	vec4 world_position = modelview_matrix * vec4(vertexPosition, 1.0);
    gl_Position = projectionMatrix *  world_position;
	step = stepsFlag;
	
	viewDirection  = normalize(vec3(world_position));
    normal          = mat3(modelview_matrix) * vertexNormal;
}
