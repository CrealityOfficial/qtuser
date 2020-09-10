#version 130
out vec4 fragmentColor;

in vec3 viewDirection;
in vec3 normal;
in vec3 worldPosition;
flat in float flag;

uniform vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 specular = vec4(0.125, 0.125, 0.125, 1.0);
uniform float specularPower = 12.0;

uniform vec3 lightDirection = vec3(0.0, 0.0, 1.0);

uniform vec4 stateColors[5];

uniform vec3 minSpace;
uniform vec3 maxSpace;
uniform float bottom; 
uniform float topVisibleHeight = 100000.0;
uniform float bottomVisibleHeight = -10000.0;

void main( void )
{
	if(worldPosition.z < bottomVisibleHeight || worldPosition.z > topVisibleHeight)
		discard;
		
	if(flag == 0.0)
		discard;
		
	vec4 color = stateColors[int(flag)];	
	
	vec3 fnormal 		  =	normalize(normal);
	vec4 specular_color   = specular;
	
	float NdotL 		  = dot(fnormal, lightDirection);
	vec4 ambientColor 	  = ambient * color;
	vec3 freflection      = normalize(((2.0 * fnormal) * NdotL) - lightDirection);
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
	
	fragmentColor = vec4(coreColor.rgb, 1.0);
}