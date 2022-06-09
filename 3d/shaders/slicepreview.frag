#version 150 core
out vec4 fragment_color;

flat in vec4 vertexFlag_es_out;
flat in vec4 vertexDrawFlag_es_out;
in vec4 vertexSmoothFlag_es_out;

in vec3 viewDirection;
// in vec3 worldNormal_es_out;
in vec3 worldPos_es_out;

flat in float segmentLength_es_out;
flat in vec3 segmentMidPos_es_out;
flat in vec3 segmentNDir_es_out;

flat in float VOPNCosValue_es_out;
flat in vec3 planeNormalNDir_es_out;
flat in vec3 worldViewNDir_es_out;

in vec4 color_test;

uniform vec4 color = vec4(0.8, 0.8, 0.8, 1.0);

uniform vec4 front_ambient = vec4(0.8, 0.8, 0.8, 1.0);
uniform vec4 front_diffuse = vec4(0.3, 0.3, 0.3, 1.0);
uniform vec4 back_ambient = vec4(0.3, 0.3, 0.3, 1.0);
uniform vec4 back_diffuse = vec4(0.3, 0.3, 0.3, 1.0);
uniform vec4 specular = vec4(0.2, 0.2, 0.2, 1.0);

uniform vec3 light_direction1 = vec3(1.0, 0.0, 1.0);

uniform int showType = 0;
uniform int animation = 0;

uniform vec4 clipValue;
uniform vec2 layershow;
uniform int layerstartflag_show;

uniform vec4 typecolors[18];
uniform vec4 speedcolors[13];
uniform vec4 nozzlecolors[6];

uniform int typecolorsshow[18];

uniform float specularPower = 1.0;

uniform vec3 eyePosition;


vec4 calSurfaceNormal(vec3 projSourcePosA, vec3 viewNDir, vec3 rotateOriginO, float majorRadius, float minorRadius, float height, vec3 heightNDir, vec3 minorAxisNDir)
{
	vec3 oaDir = projSourcePosA - rotateOriginO;
	vec3 oaNDir = normalize(oaDir);
	float loa = distance(projSourcePosA, rotateOriginO);
	float cosValueOAdMI = dot(oaNDir, minorAxisNDir);
	float lah = cosValueOAdMI * loa;

	float cosValueVdH = dot(viewNDir, heightNDir);
	float inverseFlag = step(0.0, cosValueVdH) * 2 - 1;
	heightNDir = inverseFlag * heightNDir;
	cosValueVdH = inverseFlag * cosValueVdH;

	float sinValueVdHMA = dot(viewNDir, minorAxisNDir);

	float cosValueOAdH = dot(oaNDir, heightNDir);

	float l2 = majorRadius * majorRadius;
	float s2 = minorRadius * minorRadius;
	float disL2mS2 = l2 - s2;

	float a = (sinValueVdHMA * sinValueVdHMA * disL2mS2 + (1 - cosValueVdH * cosValueVdH) * s2);
	float b = 2 * sinValueVdHMA * lah * disL2mS2 - 2 * cosValueVdH * cosValueOAdH * loa * s2;
	float c = lah * lah * disL2mS2 + s2 * (loa * loa * (1 - cosValueOAdH * cosValueOAdH) - l2);

	float delta = b * b - 4 * a * c;

	vec4 resultNormal = vec4(0.0, 0.0, 0.0, -1.0);
	if (delta >= 0)
	{
		float sDelta = sqrt(delta);
		float lv1 = (-b + sDelta) /  (2 * a);
		float lv2 = (-b - sDelta) /  (2 * a);

		float lo1 = lv1 * cosValueVdH + loa * cosValueOAdH;
		float lo2 = lv2 * cosValueVdH + loa * cosValueOAdH;

		float lv1N2Flag = step(lv2, lv1);
		float lvMax = lv1N2Flag * lv1 + (1 - lv1N2Flag) * lv2;
		float loMax = lv1N2Flag * lo1 + (1 - lv1N2Flag) * lo2;

		vec3 lvNDir = viewNDir;
		vec3 hNDir = heightNDir;

		vec4 targetPos = vec4(projSourcePosA, -1.0);
		vec4 targetPosTemp = vec4(projSourcePosA + lvMax * lvNDir, 1.0);

		float halfHeight = height / 2;
		if (lo1 * lo2 > 0)
		{
			float abslo1 = abs(lo1);
			float abslo2 = abs(lo2);
			if (abslo1 > halfHeight && abslo2 > halfHeight)
			{
				resultNormal.w = -1;
			}
			else if (abslo1 > halfHeight || abslo2 > halfHeight)
			{
				if (lo1 > 0)
					resultNormal = vec4(hNDir, 1.0);
				else
					targetPos = targetPosTemp;
			}
			else
			{
				targetPos = targetPosTemp;
			}
		}
		else
		{
			targetPos = targetPosTemp;
		}

		float biggerFlag = step(-1.0, targetPos.w);
		resultNormal = biggerFlag * vec4(normalize(targetPos.xyz - (rotateOriginO + heightNDir * loMax)), 1.0) + (1 - biggerFlag) * resultNormal;
	}

	return resultNormal;
}

vec4 directLight(vec3 light_dir, vec3 fnormal, vec4 diffuse_color, vec4 core_color)
{
	float NdotL 		  = max(dot(fnormal, light_dir), 0.0);
	diffuse_color         = diffuse_color * core_color;
	vec4 total_diffuse    = NdotL * diffuse_color;
	
//	/*
	vec3 freflection      = reflect(-light_dir, fnormal);
	vec3 fViewDirection   = normalize(viewDirection);
	float RdotV           = max(0.0, dot(freflection, fViewDirection)); 
	vec4 specularColor    = specular * pow( RdotV, specularPower) * core_color;
//	*/
	
	return total_diffuse + specularColor;
}

void main( void )
{
	if(vertexFlag_es_out.x < clipValue.x || vertexFlag_es_out.x > clipValue.y)
		discard;
		
	if(vertexFlag_es_out.x == clipValue.y && (vertexFlag_es_out.y < clipValue.z || vertexFlag_es_out.y > clipValue.w))
		discard;
		
	if(vertexFlag_es_out.x < layershow.x || vertexFlag_es_out.x > layershow.y)
		discard;
		
	if(typecolorsshow[int(vertexDrawFlag_es_out.y)] == 0)
		discard;
	
	vec4 core_color = vec4(0.5, 0.5, 0.5, 1.0);
	
	if(showType == 0)
	{
		int stype = int(vertexDrawFlag_es_out.x);
		core_color = speedcolors[stype];
	}
	else if(showType == 1)
		core_color = typecolors[int(vertexDrawFlag_es_out.y)];
	else if(showType == 2)
		core_color = nozzlecolors[int(vertexDrawFlag_es_out.z)];
	
	if(layerstartflag_show == 1 && vertexSmoothFlag_es_out.x > 0.1 && vertexSmoothFlag_es_out.x < 0.3)
		core_color = vec4(1.0, 1.0, 1.0, 1.0);

	float expandTimes = 1.0;
	float majorRadius = vertexFlag_es_out.z / 2 * expandTimes;
	float minorRadius = vertexFlag_es_out.w / 2 * expandTimes;
	vec3 minorAxisNDir = vec3(0.0, 0.0, 1.0);

	vec3 o2FragmentNDir = normalize(worldPos_es_out - segmentMidPos_es_out); 
	vec3 o2targetNDir = normalize(cross(worldViewNDir_es_out, cross(o2FragmentNDir, worldViewNDir_es_out)));
	vec3 normalTargetPos = segmentMidPos_es_out + o2targetNDir * (distance(worldPos_es_out, segmentMidPos_es_out) * dot(o2FragmentNDir, o2targetNDir));
	vec4 surfaceNormal    = calSurfaceNormal(normalTargetPos, worldViewNDir_es_out, segmentMidPos_es_out, majorRadius, minorRadius, segmentLength_es_out, segmentNDir_es_out, minorAxisNDir);

	if (surfaceNormal.w < 0)
		discard;

	// vec3 fnormal 		  =	normalize(worldNormal_es_out);
	vec3 fnormal 		  =	surfaceNormal.xyz;
	vec4 ambient_color 	  = front_ambient;
	vec4 diffuse_color    = front_diffuse;
	
	ambient_color 		  = ambient_color * core_color;
	
	vec4 light_color1     = directLight(light_direction1, fnormal, diffuse_color, core_color);
	
	core_color = ambient_color + light_color1;
	core_color.a = color.a;

	if(animation > 0)
	{
		if(vertexFlag_es_out.x == clipValue.y)
		{
			core_color += vec4(0.3, 0.3, 0.3, 0.0);
		}
		else
		{
			core_color -= vec4(0.3, 0.3, 0.3, 0.0);
		}
	}
	
	fragment_color = core_color;
	// fragment_color = color_test;
}
