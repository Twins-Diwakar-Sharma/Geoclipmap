#version 400

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 merge;

out TCSvar{
	vec2 merge;
} tcsVar;

void main()
{
	gl_Position = vec4(pos.x,0,pos.y,1.0);
	tcsVar.merge = merge;
}

