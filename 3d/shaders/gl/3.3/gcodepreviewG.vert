#version 150 core

in vec3 vertexPosition;
in vec3 endVertexPosition;
in vec3 vertexNormal;
in vec2 stepsFlag;
in float visualTypeFlags;
in float lineWidth;

out vec3 startVertexVS;
out vec3 endVertexVS;
out vec3 vNormalVS;
flat out vec2 stepVS;
flat out float visualTypeVS;
flat out float lineWidthPerStep;

void main( void )
{	
	startVertexVS = vertexPosition;
	endVertexVS = endVertexPosition;
	vNormalVS = vertexNormal;
	stepVS = stepsFlag;
	visualTypeVS = visualTypeFlags;
	lineWidthPerStep = lineWidth;
}
