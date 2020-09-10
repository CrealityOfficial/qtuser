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
uniform int showState = 0;
uniform float zcha = 0.01;

uniform float state;
uniform vec4 stateColors[4];

void main( void )
{
	if(worldPosition.z < bottomVisibleHeight || worldPosition.z > topVisibleHeight)
		discard;

	vec4 color = stateColors[int(state)];	
	if(error == 1.0 || !gl_FrontFacing)
		color = stateColors[3];
	
	vec3 fnormal 		  =	normalize(normal);
	vec3 fgnormal 		  =	normalize(gnormal);
	vec4 specular_color   = specular;
	
	float NdotL 		  = max(dot(fnormal, lightDirection), 0.0);
	vec4 ambientColor 	  = ambient * color;
	vec3 freflection      = reflect(-lightDirection, fnormal);
	vec3 fViewDirection   = normalize(viewDirection);
	float RdotV           = max(0.0, dot(freflection, fViewDirection)); 
	vec4 diffuseColor     = NdotL * diffuse * color;
	vec4 specularColor    = specular * pow( RdotV, specularPower);
	vec4 coreColor = ambientColor + diffuseColor + specularColor;
	
	if(worldPosition.x < minSpace.x || worldPosition.y < minSpace.y || worldPosition.z < minSpace.z || worldPosition.x > maxSpace.x || worldPosition.y > maxSpace.y || worldPosition.z > maxSpace.z)
	{
		coreColor.g += 0.4;
	}
	
	if( abs(worldPosition.z - bottom) < 0.01 )
	{
		coreColor.g += 0.4;
	}
	
	if(showState > 0)
	{
		if(dot(fgnormal, vec3(0.0, 0.0, -1.0)) > supportCos)
		{
			coreColor.r += 0.4;
		}
		
//		if(state == 1)
		{
			if(abs(worldPosition.z - worldWater.z) < zcha)
			{
				coreColor = vec4(0.1, 0.1, 0.1, 1.0);
			}
		}
	}
	
	fragmentColor = vec4(coreColor.rgb, 1.0);
}