#version 400

struct Cam
{
	vec4 ori;
	vec3 pos;
};

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 norm;

out vec2 outTex;
out vec3 outPos;
out vec3 outNorm;

uniform mat4 projection;
uniform mat4 transform;

uniform Cam cam;

vec4 quatRotate(vec4 action, vec4 victim)
{
	float ar = action.x;	float br = victim.x;
	vec3 av = action.yzw;	vec3 bv = victim.yzw;
	return vec4(ar*br - dot(av,bv), ar*bv + br*av + cross(av,bv));
}

void main() 
{
	outTex = tex;
	vec4 worldPos =  transform * vec4(pos,1.0);
	
	vec3 viewPos = worldPos.xyz - cam.pos;
	vec4 quatView = vec4(0,viewPos);
	vec4 oriQuat = vec4(cam.ori.x, -cam.ori.yzw);
	vec4 oriQuatInv = vec4(cam.ori);

	quatView = quatRotate(quatView, oriQuatInv);
	quatView = quatRotate(oriQuat, quatView);

	vec4 projectedPos = projection * vec4(quatView.yzw,1.0);

	gl_Position = projectedPos;
	outPos = vec3(worldPos);

	outNorm = (transform*vec4(norm,0.0)).xyz;

}

