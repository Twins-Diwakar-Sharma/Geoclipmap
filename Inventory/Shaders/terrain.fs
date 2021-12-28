#version 400

out vec4 outColor;

//in vec3 fragNorm;
in float fragTerrainSize;
in vec3 fragWorldPos;
in vec3 fragObjectPos;

uniform vec3 lightDir;
uniform vec3 lightColor;

uniform sampler2D heightmap;

float heightFromTex(float x, float z)
{
		float terrainSize = fragTerrainSize;
		vec2 texPos = vec2( (x + terrainSize/2)/terrainSize, (z + terrainSize/2)/terrainSize );
		vec4 heightColor = texture(heightmap,texPos);
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

	vec3 fractionalPos;
	fractionalPos.x = fragObjectPos.x - int(fragObjectPos.x); 
	fractionalPos.z = fragObjectPos.z - int(fragObjectPos.z);

	vec3 eightPos;
	eightPos = fractionalPos*4;
	eightPos.x = eightPos.x - int(eightPos.x);
	eightPos.z = eightPos.z - int(eightPos.z);

	if((fractionalPos.x > -1 && fractionalPos.x < -1+0.01) || (fractionalPos.z > -1 && fractionalPos.z < -1 + 0.01) )
		outColor = vec4(0.7,0.4,0,1);
	else if( (eightPos.x > -1 && eightPos.x < -1 + 0.02) || (eightPos.z > -1 && eightPos.z < -1 + 0.02) || 
			  (eightPos.x > 1-0.02 && eightPos.x < 1) || (eightPos.z > 1-0.02 && eightPos.z < 1) )
		outColor = vec4(0,0.2,0.7,1);
	else
		outColor = vec4(0.5f, 0.9f, 0.5f, 1.0f);
		 
}