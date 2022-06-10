#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec4 fcolor;

void main() 
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(ver_attribute_position, 1.0);
	fcolor = vertexColor;
}
