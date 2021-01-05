#version 130

out vec4 fragColor;
flat in vec2 flag;
uniform float visible;
uniform vec4 showcolor;
uniform vec4 linecolor;

void main() 
{
	if((int(flag.x) == 0) || (int(flag.y) == 0))
	{
		fragColor = showcolor;
	}
	else
	{
		if((((int(flag.x/10.0))%4 > 0) || ((int(flag.y/10.0))%4 > 0)) && (visible == 0.0))
			discard;
		
		fragColor = linecolor;
	}
	

/*
	if (visible == 0.0)
	{
		discard;
	}
	else
	{
		if(((int(flag.x/10.0))%4 > 0) || ((int(flag.y/10.0))%4 > 0))
			//fragColor = showcolor;
			discard;
		else	
			fragColor = linecolor;
	}
	*/
}
