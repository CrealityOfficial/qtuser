#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 normalDir;
out vec3 viewDir;
out vec3 lightDir;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 u_lightDir = vec3(0.0, 1.0, 0.8);


void main() 
{
   vec3 view_position = vec3(viewMatrix * modelMatrix * vec4(vertexPosition, 1.0));
	viewDir = normalize(-view_position);

   normalDir = normalize(mat3(viewMatrix * modelMatrix) * vertexNormal);

   vec3 light = mat3(viewMatrix) * u_lightDir;
   lightDir = normalize(light);

   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
