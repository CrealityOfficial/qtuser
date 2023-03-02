#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in float facesIndex;

out vec2 texCoord;
out float dirIndex;
out vec3 normal;

uniform mat4 modelViewProjection;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
   texCoord = vertexTexCoord;
   dirIndex = facesIndex;
   normal = vertexNormal;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
