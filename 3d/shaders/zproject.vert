#version 150 core

in vec3 vertexPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float projectZ = 0.01;

void main( void )
{
    vec4 tworldPosition = modelMatrix * vec4(vertexPosition, 1.0);
	tworldPosition.z = projectZ;
    gl_Position = projectionMatrix * viewMatrix *  tworldPosition;
}
