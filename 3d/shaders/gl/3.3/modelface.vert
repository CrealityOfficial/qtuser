#version 150 core
/* modelwireframe.vert */

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 vertexColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 viewDirection;
out vec3 normal;
out vec3 gnormal;
out vec3 worldPosition;
out vec2 varyUV;
out vec3 gcolor;

void main( void )
{
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    vec4 tworldPosition = modelViewMatrix * vec4(vertexPosition, 1.0);
    gl_Position = projectionMatrix *  tworldPosition;

    viewDirection  = normalize(vec3(-tworldPosition));
    mat3 normalMatrix = mat3(modelViewMatrix);

    normal          = normalMatrix * vertexNormal;
    gnormal        = mat3(modelMatrix) * vertexNormal;
		
    worldPosition   = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	varyUV = vertexTexCoord;
    gcolor      = vertexColor;

}
