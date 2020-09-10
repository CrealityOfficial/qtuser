#version 130

out vec4 fragColor;
flat in vec2 flag;
uniform float visible;

void main() 
{
	if((((int(flag.x/10.0))%4 > 0) || ((int(flag.y/10.0))%4 > 0)) && (visible == 0.0))
		discard;
		
	fragColor = vec4(0.8, 0.8, 0.8, 1.0);
}
