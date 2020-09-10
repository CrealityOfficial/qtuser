#version 130

flat in vec4 passColor;
in vec3 passVert;

out vec4 fragColor;

void main()
{
	fragColor = passColor;
}