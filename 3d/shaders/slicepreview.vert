#version 130

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexFlag;
in vec4 vertexDrawFlag;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 normal;
flat out vec2 flag;
flat out vec4 drawFlag;
out vec3 viewDirection;

void main( void )
{	
	mat4 modelview_matrix = viewMatrix * modelMatrix;
	vec4 world_position = modelview_matrix * vec4(vertexPosition, 1.0);
    gl_Position = projectionMatrix *  world_position;
	flag = vertexFlag;
	drawFlag = vertexDrawFlag;
	
	viewDirection  = normalize(vec3(-world_position));
    normal          = mat3(modelview_matrix) * vertexNormal;
}