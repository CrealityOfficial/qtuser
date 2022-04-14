#version 150 core

out vec4 fragColor;

in vec2 texcoord;
in vec3 normal;

uniform vec2 imageshape;
uniform vec2 imagebili;
uniform vec2 platformsize;

uniform sampler2D shapeTexture;
uniform vec4 color;
uniform vec4 logcolor;
uniform int visible;
uniform int colorVisible;

void main() 
{
	vec4 coloruse = color;
	vec4 finalcolor = color;
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
			if(coloruse.a > 0.0 && colorVisible <= 0)
			{
				coloruse = logcolor;
			}
		}
	}
	
	if(coloruse.a > 0.0)
		finalcolor = coloruse;
		
	vec3 fgnormal 		  =	normalize(normal);
	if(dot(fgnormal, vec3(0.0, 0.0, -1.0)) > 0)
	{
		finalcolor.a = 0.0;
	}
	fragColor = finalcolor;
	
}
