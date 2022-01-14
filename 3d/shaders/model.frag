#version 150 core
out vec4 fragmentColor;

in vec3 viewDirection;
in vec3 normal;
in vec3 gnormal;
in vec3 worldPosition;
in vec3 worldWater;

uniform vec4 ambient = vec4(0.8, 0.8, 0.8, 1.0);
uniform vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 specular = vec4(0.8, 0.8, 0.8, 1.0);
uniform float specularPower = 12.0;
uniform vec3 lightDirection = vec3(0.0, 0.0, 1.0);

uniform int lightingEnable = 1;
uniform float transparency = 1.0;

uniform vec3 minSpace;
uniform vec3 maxSpace;
uniform float bottom; 
uniform float topVisibleHeight = 100000.0;
uniform float bottomVisibleHeight = -10000.0;
uniform float error;
uniform float supportCos = 0.5;

uniform int hoverState = 0;
uniform int waterState = 0;
uniform int fanzhuan = 0;
uniform int checkscope = 1;

uniform float zcha = 0.01;

uniform float state;
uniform float nozzle;
uniform vec4 stateColors[6];
uniform vec4 customColor;

bool frontFacing()
{
        vec3 fdx = dFdx(worldPosition);
        vec3 fdy = dFdy(worldPosition);
        return dot(gnormal, cross(fdx, fdy)) > 0.0;
} 

vec4 directLight(vec3 light_dir, vec3 fnormal, vec4 core_color, vec4 ambient_color, vec4 diffuse_color, vec4 specular_color)
{
	float NdotL 		  = max(dot(fnormal, light_dir), 0.0);
	ambient_color 	  	  = ambient_color * core_color;

	vec3 freflection      = reflect(-light_dir, fnormal);
	vec3 fViewDirection   = normalize(viewDirection);
	float RdotV           = max(0.0, dot(freflection, fViewDirection)); 
	
	diffuse_color		  = NdotL * diffuse_color * core_color;
	specular_color        = specular_color * pow( RdotV, specularPower) * core_color;
	
	return lightingEnable * (ambient_color + diffuse_color + specular_color) + (1 - lightingEnable) * core_color;
}

void main( void )
{
	if(checkscope > 0 && (worldPosition.z < bottomVisibleHeight || worldPosition.z > topVisibleHeight))
		discard;

	int stateInt = int(state);
	vec4 color;
	if (stateInt < 5)
		color = stateColors[stateInt];
	else
		color = customColor;
	
	if(error == 1.0)
		color = stateColors[3];
	
	vec3 fnormal 		  =	normalize(normal);
	vec4 ambient_color 	  = ambient;
	vec4 diffuse_color    = diffuse;
	vec4 specular_color   = specular;
	
	vec3 lightDir = normalize(lightDirection);
	vec4 coreColor = directLight(lightDir, fnormal, color, ambient_color, diffuse_color, specular_color);
	
	vec3 fgnormal 		  =	normalize(gnormal);
	
	if(checkscope > 0)
	{
		if(worldPosition.x < minSpace.x || worldPosition.y < minSpace.y || worldPosition.z < minSpace.z || worldPosition.x > maxSpace.x || worldPosition.y > maxSpace.y || worldPosition.z > maxSpace.z)
		{
			coreColor *= 0.3;
			coreColor.r += 0.6;
		}
		if( abs(worldPosition.z - bottom) < 0.05 )
		{
			coreColor *= 0.3;
			coreColor.r += 0.6;
		}
	}
	
	
	if(hoverState > 0)
	{
		float v = dot(fgnormal, vec3(0.0, 0.0, -1.0));
		if(v >= supportCos)
		{
			coreColor.r += 0.8 * v;
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
	
	if(!frontFacing())
	{
		coreColor = stateColors[5];   // vec3(0.65, 0.75, 0.95) - coreColor.rgb;
		coreColor.a = 1.0;
	}

    //int fz = fanzhuan % 2;
    //   
    //if(fz == 0)
    //{
    //    if(! frontFacing())
    //    {
    //        //coreColor = stateColors[5];   // vec3(0.65, 0.75, 0.95) - coreColor.rgb;
	//		coreColor.a = 1.0;
    //    }
    //}
    //else
    //{
    //    if(frontFacing())
    //    {
    //        //coreColor = stateColors[5];   // vec3(0.65, 0.75, 0.95) - coreColor.rgb;
	//		coreColor.a = 1.0;
    //    }
    //}
   	
	float vnValue = viewDirection.x * fnormal.x + viewDirection.y * fnormal.y + viewDirection.z * fnormal.z;
	if (vnValue < -0.2)
		coreColor = vec4(1.0, 0.54, 0.0, 1.0);

	fragmentColor = vec4(coreColor.rgb, transparency);
}
