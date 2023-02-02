#version 150 core

in vec4 clip; 
in vec3 worldPosition;

out vec4 fragColor;

uniform vec3 globalNormal;
uniform vec4 color;
uniform sampler2D depthTexture;
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D faceTexture;
uniform ivec2 faceRange;

void main() 
{
	vec3 ndc = clip.xyz / clip.w;
   	ndc = ndc * 0.5 + 0.5;
	vec2 uv = ndc.xy;

	// float d = texture(depthTexture, ndc.xy).x;

	// float currentDepth = ndc.z;

	// float threshold = 0.005;

	// float k = smoothstep(0.0, threshold, abs(d - currentDepth));
	// k = pow(1.0-k, 150.0);

	// fragColor = vec4(color.rgb, color.a + k);

	//两个面法线越接近，阈值越小
	vec3 dstNormal = texture(normalTexture, uv).xyz;
	float factor = dot(normalize(dstNormal), normalize(globalNormal));
	factor = max(1.0 - abs(factor), 0.3);
	float threshold = 10.0 * factor;

	vec3 dstPosition = texture(positionTexture, uv).xyz;
	float length = distance(dstPosition, worldPosition);
	float k = smoothstep(0.0, threshold, length);
	k = pow(1.0-k, 100.0);

	//排除非当前选中模型
	vec4 codedFace = texture(faceTexture, uv);
	codedFace *= 255.0;
	float faceId = (16777216.0 * codedFace.a + 65536.0 * codedFace.b + 256.0 * codedFace.g + codedFace.r);
	// if (faceRange.x <= faceId && faceId < faceRange.x+faceRange.y) {
	// } else {
	// 	k = 0.0;
	// }
	float isTarget = step(float(faceRange.x), faceId);
	isTarget *= step(faceId, float( faceRange.x+faceRange.y));
	k *= isTarget;

	fragColor = vec4(color.rgb, color.a + k);
}
