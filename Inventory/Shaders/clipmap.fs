#version 400

out vec4 outColor;

//in vec3 fragNorm;
in float fragTerrainSize;
in vec3 fragWorldPos;
in vec3 fragObjectPos;
in vec2 merge;

uniform vec3 lightDir;
uniform vec3 lightColor;

uniform sampler2D normalmap;

in vec2 uv; // just for debug


float heightFromTex(float x, float z)
{
		float terrainSize = fragTerrainSize;
		vec2 texPos = vec2( (x + terrainSize/2)/terrainSize, (z + terrainSize/2)/terrainSize );
		vec4 heightColor = texture(normalmap,texPos);
		float ans = ((heightColor.r + heightColor.g + heightColor.b)/3.0)*2 - 1;
		ans = ans * terrainSize/4;
		return ans;
		
}

void main()
{
		vec3 worldPos = fragWorldPos;

		float hL = heightFromTex(worldPos.x-1, worldPos.z);
		float hR = heightFromTex(worldPos.x+1, worldPos.z);
		float hD = heightFromTex(worldPos.x, worldPos.z-1);
		float hU = heightFromTex(worldPos.x, worldPos.z+1);


		vec3 normFromTex = normalize(vec3(hL-hR, 2.0, hD-hU));

		vec3 fragNorm = normFromTex;

	vec3 toLight = normalize(-lightDir);
	float diffuse = max(dot(fragNorm, toLight) , 0.2);	
	outColor =  vec4(0.5f,0.9f,0.5f,1.0f)*vec4(diffuse*lightColor,1);
	float u = uv[0], v = uv[1];
	if( (u >=0 && u < 0.005) || ( v >= 0 && v < 0.005) || ( u > 0.995 && u <= 1) || ( v > 0.995 && v <= 1))
	{
		
			outColor = vec4(1,0,0,1);

	}
//	else
//	{
//		outColor = vec4(0.5,0.8,0.5,1);
//	}
}