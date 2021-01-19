#version 150 core

out vec4 fragColor;

in vec2 texcoord;

uniform vec2 imageshape;
uniform vec2 imagebili;
uniform vec2 platformsize;

uniform sampler2D shapeTexture;
uniform vec4 color;
uniform int visible;

void main() 
{
	vec4 coloruse = color;
	fragColor = color;

	if(visible == 1)
	{
		vec2 per = platformsize / imageshape;
		float minv = per.x < per.y ? per.x : per.y;
		per = per / minv;
	
		vec2 bili = per * imagebili;
		//vec2 bili = vec2(1.0, 1.0) * imagebili;
		
		vec2 texuse = (texcoord - vec2(0.5, 0.5)) * bili + vec2(0.5, 0.5);
			
		if(texuse.x >= 0.0 && texuse.x <= 1.0 && texuse.y >= 0.0 && texuse.y <= 1.0)
		{
			coloruse = texture(shapeTexture, texuse);
			if(coloruse.a > 0.0)
			{
				coloruse.a = 0.5;
			}
		}
	}
	if(coloruse.a > 0.0)
		fragColor = coloruse;
}
