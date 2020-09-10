#version 130
out vec4 fragmentColor;

in vec3 viewDirection;
in vec3 normal;

uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 ambient = vec4(0.7, 0.7, 0.7, 1.0);
uniform vec4 diffuse = vec4(0.3, 0.3, 0.3, 1.0);
uniform vec4 specular = vec4(0.5, 0.5, 0.5, 1.0);
uniform float specularPower = 128.0;

uniform vec3 lightDirection = vec3(0.0, 0.0, 1.0);

void main( void )
{
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
	fragmentColor = vec4(coreColor.rgb, 1.0);
}