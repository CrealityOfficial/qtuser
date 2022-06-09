#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexFlag;
in vec4 vertexDrawFlag;
in vec4 vertexSmoothFlag;

out vec3 worldPos_vs_out;
flat out vec4 vertexFlag_vs_out;
flat out vec4 vertexDrawFlag_vs_out;
out vec4 vertexSmoothFlag_vs_out;

uniform mat4 modelMatrix;


void main( void )
{	
	vertexFlag_vs_out = vertexFlag;
	vertexDrawFlag_vs_out = vertexDrawFlag;
	vertexSmoothFlag_vs_out = vertexSmoothFlag;

	worldPos_vs_out = vec3(modelMatrix * vec4(vertexPosition, 1.0));
}
