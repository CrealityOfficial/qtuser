#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;

in vec3 worldPos;
in vec2 stepsFlag;

out vec2 flags;

uniform mat4 modelViewProjection;

void main() 
{
   flags = stepsFlag;
   gl_Position = modelViewProjection * vec4(worldPos + vertexPosition, 1.0);
}
