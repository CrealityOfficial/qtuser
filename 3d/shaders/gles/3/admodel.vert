#version 300 es
precision mediump float;

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 viewDirection;
out vec3 normal;
out vec3 worldPosition;

void main( void )
{
    mat4 modelViewMatrix  = viewMatrix * modelMatrix;
    vec4 tworldPosition   = modelViewMatrix * vec4(vertexPosition, 1.0);
    gl_Position           = projectionMatrix *  tworldPosition;

    viewDirection         = normalize(vec3(-tworldPosition));
    mat3 normalMatrix     = mat3(modelViewMatrix);

    normal          	  = normalMatrix * vertexNormal;
    worldPosition   	  = vec3(modelMatrix * vec4(vertexPosition, 1.0));
}
