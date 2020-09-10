#version 130

flat in vec4 passColor;
in vec3 passVert;
in float flag;

out vec4 fragColor;

void main()
{
	if(flag == 0.0)
		discard;
	fragColor = passColor;
}