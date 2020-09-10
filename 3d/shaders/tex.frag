#version 130

out vec4 fragColor;

uniform sampler2D shapeTexture;

in vec2 texcoord;
void main() 
{
	vec4 color = texture(shapeTexture, texcoord);
	if(color.w == 1.0) discard;
	fragColor = vec4(color.xyz, 1.0);  
}
