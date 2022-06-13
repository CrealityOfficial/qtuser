/* Tex Vertex*/

attribute vec3 vertexPosition;
attribute vec2 vertexTexcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
varying vec2 texcoord;

void main() 
{
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;
   texcoord = vertexTexcoord;
}
