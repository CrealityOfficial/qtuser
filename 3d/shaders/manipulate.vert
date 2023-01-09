#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 normal;
out vec3 viewDirection;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() 
{
   vec3 view_position = vec3(viewMatrix * modelMatrix * vec4(vertexPosition, 1.0));
   viewDirection  = normalize(view_position);
   normal = mat3(viewMatrix * modelMatrix) * vertexNormal;
   // normal = -viewDirection;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
