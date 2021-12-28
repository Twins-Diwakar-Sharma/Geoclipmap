#version 400

layout (location = 0) in vec2 pos;

void main()
{
	gl_Position = vec4(pos.x,0,pos.y,1.0);
}

