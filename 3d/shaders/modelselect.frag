#version 130
out vec4 fragmentColor;

noperspective in vec3 viewDirection;
noperspective in vec3 normal;
noperspective in vec3 worldPosition;

uniform vec4 ambient = vec4(0.4, 0.4, 0.4, 1.0);
uniform vec4 diffuse = vec4(0.6, 0.6, 0.6, 1.0);
uniform vec4 specular = vec4(0.125, 0.125, 0.125, 1.0);
uniform float specularPower = 12.0;

uniform vec3 lightDirection = vec3(0.0, 0.0, 1.0);
uniform float topVisibleHeight = 100000.0;
uniform float bottomVisibleHeight = -10000.0;
uniform float error;

uniform float state;
uniform vec4 stateColors[4];

void main( void )
{
	if(worldPosition.z < bottomVisibleHeight || worldPosition.z > topVisibleHeight)
		discard;

	vec4 color = stateColors[int(state)];	
	if(error == 1.0)
		color = stateColors[3];
	
	vec3 fnormal 		  =	normalize(normal);
	vec4 specular_color   = specular;
	
	float NdotL 		  = max(dot(fnormal, lightDirection), 0.0);
	vec4 ambientColor 	  = ambient * color;
	vec3 freflection      = reflect(-lightDirection, fnormal);
	vec3 fViewDirection   = normalize(viewDirection);
	float RdotV           = max(0.0, dot(freflection, fViewDirection)); 
	vec4 diffuseColor     = NdotL * diffuse * color;
	vec4 specularColor    = specular * pow( RdotV, specularPower);
	vec4 coreColor = ambientColor + diffuseColor + specularColor;
	
	coreColor.g += 0.4;
	coreColor.r += 0.4;
	
	fragmentColor = vec4(coreColor.rgb, 1.0);
}