#version 300 es
precision mediump float;

in vec3 vertexPosition;
in vec2 vertexFlag;
uniform mat4 modelViewProjection;

flat out vec2 flag;
void main() 
{
   gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
   flag = vertexFlag;
}
