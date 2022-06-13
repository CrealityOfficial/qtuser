/* PrinterBottom Vertex*/

varying vec3 vertexPosition;
varying vec3 vertexNormal;
varying vec2 vertexTexcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

varying vec2 texcoord;
varying vec3 normal;

void main() 
{
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
   texcoord = vertexTexcoord;
   
   mat3 normalMatrix = mat3(viewMatrix);
   normal          = normalMatrix * vertexNormal;
}
