#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 normalDir;
out vec3 viewDir;
out vec3 lightDir;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform int lightEnable = 1;

void main() 
{
   if (lightEnable == 1)
   {
      vec3 view_position = vec3(viewMatrix * modelMatrix * vec4(vertexPosition, 1.0));
	   viewDir = normalize(view_position);

      vec3 normal = mat3(viewMatrix * modelMatrix) * vertexNormal;
      normalDir = normalize(normal);

      vec3 light = mat3(viewMatrix) * vec3(0.0, 1.0, 0.8);
      lightDir = normalize(light);

   }
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
