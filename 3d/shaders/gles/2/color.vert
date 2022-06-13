/* Color Vertex*/

attribute vec3 vertexPosition;
attribute vec4 vertexColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

varying vec4 fcolor;

void main() 
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertexPosition, 1.0);
	fcolor = vertexColor;
}
