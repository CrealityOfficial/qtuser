#version 130
out vec4 fragmentColor;

in vec3 viewDirection;
in vec3 normal;
in vec3 gnormal;
in vec3 worldPosition;
in vec3 worldWater;

uniform vec4 ambient = vec4(0.4, 0.4, 0.4, 1.0);
uniform vec4 diffuse = vec4(0.6, 0.6, 0.6, 1.0);
uniform vec4 specular = vec4(0.125, 0.125, 0.125, 1.0);
uniform float specularPower = 12.0;

uniform vec3 lightDirection = vec3(0.0, 0.0, 1.0);
uniform vec3 minSpace;
uniform vec3 maxSpace;
uniform float bottom; 
uniform float topVisibleHeight = 100000.0;
uniform float bottomVisibleHeight = -10000.0;
uniform float error;
uniform float supportCos = 0.7;
uniform int hoverState = 0;
uniform int waterState = 0;
uniform float zcha = 0.01;

uniform float state;
uniform float nozzle;
uniform vec4 stateColors[4];

vec4 directLight(vec3 light_dir, vec3 fnormal, vec4 core_color, vec4 ambient_color, vec4 diffuse_color, vec4 specular_color)
{
	float NdotL 		  = max(dot(fnormal, light_dir), 0.0);
	ambient_color 	  	  = ambient_color * core_color;

	vec3 freflection      = reflect(-light_dir, fnormal);
	vec3 fViewDirection   = normalize(viewDirection);
	float RdotV           = max(0.0, dot(freflection, fViewDirection)); 
	
	diffuse_color		  = NdotL * diffuse_color * core_color;
	specular_color        = specular_color * pow( RdotV, specularPower);
	
	return ambient_color + diffuse_color + specular_color;
}

void main( void )
{
	if(worldPosition.z < bottomVisibleHeight || worldPosition.z > topVisibleHeight)
		discard;

	vec4 color = stateColors[int(state)];	
	if(error == 1.0)
		color = stateColors[3];
	
	vec4 back_color = color;
	back_color.rgb = vec3(1.0, 1.0, 1.0) - back_color.rgb;
	
	vec3 fnormal 		  =	normalize(normal);
	vec4 ambient_color 	  = ambient;
	vec4 diffuse_color    = diffuse;
	vec4 specular_color   = specular;
	
	vec4 coreColor = directLight(lightDirection, fnormal, color, ambient_color, diffuse_color, specular_color);
	vec4 coreColor_b = directLight(lightDirection, -fnormal, back_color, ambient_color - vec4(0.2, 0.2, 0.2, 0.0), diffuse_color, specular_color);
	
	vec3 fgnormal 		  =	normalize(gnormal);
	
	if(worldPosition.x < minSpace.x || worldPosition.y < minSpace.y || worldPosition.z < minSpace.z || worldPosition.x > maxSpace.x || worldPosition.y > maxSpace.y || worldPosition.z > maxSpace.z)
	{
		coreColor.g += 0.4;
		coreColor_b.g += 0.2;
	}
	
	if( abs(worldPosition.z - bottom) < 0.01 )
	{
		coreColor.g += 0.4;
		coreColor_b.g += 0.2;
	}
	
	if(hoverState > 0)
	{
		if(dot(fgnormal, vec3(0.0, 0.0, -1.0)) > supportCos)
		{
			coreColor.r += 0.4;
			coreColor_b.r += 0.2;
		}
		
		if(waterState == 1)
		{
			if(abs(worldPosition.z - worldWater.z) < zcha)
			{
				coreColor = vec4(0.1, 0.1, 0.1, 1.0);
			}
		}
	}
	
	coreColor.rgb = coreColor.rgb + vec3(0.1, -0.1, 0.0) * nozzle;
	coreColor_b.rgb = coreColor_b.rgb + vec3(0.1, -0.1, 0.0) * nozzle;
	
	if(gl_FrontFacing)
    {
        fragmentColor = vec4(coreColor.rgb, 1.0);
    }
    else
    {
        fragmentColor = vec4(coreColor_b.rgb, 0.4);
    }
	
//	fragmentColor = vec4(coreColor.rgb, 1.0);
}