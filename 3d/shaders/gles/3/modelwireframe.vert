#version 300 es
precision mediump float;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
		
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 viewDirectionVS;
out vec3 normalVS;
out vec3 gnormalVS;
out vec3 worldPositionVS;
out vec2 varyUVVS;

void main( void )
{
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    vec4 tworldPosition = modelViewMatrix * vec4(vertexPosition, 1.0);
    gl_Position = projectionMatrix *  tworldPosition;

    viewDirectionVS  = normalize(vec3(-tworldPosition));
    mat3 normalMatrix = mat3(modelViewMatrix);

    normalVS          = normalMatrix * vertexNormal;
    gnormalVS        = mat3(modelMatrix) * vertexNormal;
		
    worldPositionVS   = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	varyUVVS = vertexTexCoord;
}
