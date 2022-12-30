#version 150 core

out vec4 fragColor;

in vec3 normal;
in vec3 viewDirection;

uniform vec4 color;
uniform vec4 changecolor;
uniform int mt;

uniform vec4 ambient = vec4(0.6, 0.6, 0.6, 1.0);
uniform vec4 diffuse = vec4(0.6, 0.6, 0.6, 1.0);
uniform vec4 specular = vec4(0.6, 0.6, 0.6, 1.0);
uniform float specularPower = 4.5;
uniform vec3 lightDirection = vec3(1.0, 0.0, 1.0);

uniform float state;

vec4 directLight(vec3 light_dir, vec3 fnormal, vec4 core_color, vec4 ambient_color, vec4 diffuse_color, vec4 specular_color)
{
	float NdotL 		  = max(dot(fnormal, light_dir), 0.0);
	ambient_color 	  	  = ambient_color * core_color;

	vec3 freflection      = reflect(-light_dir, fnormal);
	vec3 fViewDirection   = normalize(viewDirection);
	float RdotV           = max(0.0, dot(freflection, fViewDirection)); 
	
	diffuse_color		  = NdotL * diffuse_color * core_color;
	specular_color        = specular_color * pow( RdotV, specularPower) * core_color;
	
	return ambient_color + diffuse_color + specular_color;
}

void main() 
{
	vec4 coreColor;
	if(mt == 0)
	{
		coreColor = color * state;
	}
	else if(mt == 1)
	{
		coreColor = color + changecolor * state;
	}

	vec3 fnormal = normalize(normal);
	vec4 ambient_color = ambient;
	vec4 diffuse_color = diffuse;
	vec4 specular_color = specular;

	vec3 lightDir = normalize(lightDirection);
	float normalCos = dot(fnormal, lightDir);
	coreColor = directLight(lightDir, fnormal, coreColor, ambient_color, diffuse_color, specular_color);
		
	fragColor = coreColor;
}
