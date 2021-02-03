#version 150 core
out vec4 fragment_color;

flat in vec2 flag;
uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

uniform vec4 clipValue;
uniform vec2 layershow;

void main( void )
{
	if(flag.x < clipValue.x || flag.x > clipValue.y)
		discard;
		
	if(flag.x == clipValue.y && (flag.y < clipValue.z || flag.y > clipValue.w))
		discard;
		
	if(flag.x < layershow.x || flag.x > layershow.y)
		discard;
	
	vec4 core_color = color;
	fragment_color = core_color;
}
