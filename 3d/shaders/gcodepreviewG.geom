#version 150 core

layout (points) in;
layout (triangle_strip, max_vertices = 10) out;

in vec3 startVertexVS[1];
in vec3 endVertexVS[1];
in vec3 vNormalVS[1];
flat in vec2 stepVS[1];
flat in float visualTypeVS[1];

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 normal;
flat out vec2 step;
flat out float visualType;
out vec3 viewDirection;

void combindVertex(vec3 pos, vec3 norm)
{
	vec4 world_position = viewMatrix * vec4(pos, 1.0);
	gl_Position = projectionMatrix *  world_position;
	normal = mat3(viewMatrix) * norm;
	step = stepVS[0];
	visualType = visualTypeVS[0];
	viewDirection  = normalize(vec3(world_position));
	EmitVertex();
}

void main( void )
{	
	vec3 norms[8];
	norms[0] = vec3(0.0, 0.0, 1.0);
	norms[1] = vNormalVS[0];
	norms[2] = vec3(0.0, 0.0, -1.0);
	norms[3] = - vNormalVS[0];
	norms[4] = vec3(0.0, 0.0, 1.0);
	norms[5] = vNormalVS[0];
	norms[6] = vec3(0.0, 0.0, -1.0);
	norms[7] = - vNormalVS[0];
	
	vec3 poses[8];
	poses[0] = startVertexVS[0] + 0.05 * norms[0];
	poses[1] = startVertexVS[0] + 0.1  * norms[1];
	poses[2] = startVertexVS[0] + 0.05 * norms[2];
	poses[3] = startVertexVS[0] + 0.1  * norms[3];
	poses[4] = endVertexVS[0]   + 0.05 * norms[4];
	poses[5] = endVertexVS[0]   + 0.1  * norms[5];
	poses[6] = endVertexVS[0]   + 0.05 * norms[6];
	poses[7] = endVertexVS[0]   + 0.1  * norms[7];
	
	combindVertex(poses[0], norms[0]);
	combindVertex(poses[4], norms[4]);
	combindVertex(poses[1], norms[1]);
	combindVertex(poses[5], norms[5]);
	combindVertex(poses[2], norms[2]);
	combindVertex(poses[6], norms[6]);
	combindVertex(poses[3], norms[3]);
	combindVertex(poses[7], norms[7]);
	combindVertex(poses[0], norms[0]);
	combindVertex(poses[4], norms[4]);
	
	EndPrimitive();
}

