#version 400

out vec4 fragColor;
in vec2 outTex;
in vec3 outPos;
in vec3 outNorm;

uniform sampler2D albedo;
uniform vec3 lightDir;
uniform vec3 lightColor;

void main()
{
	float st = 0.1;
	vec3 ambeint = st * lightColor;
   
    vec3 toLight = normalize(-lightDir);
	vec3 norm = normalize(outNorm);

	float diff = max(dot(norm, toLight),0.0);
	vec3 diffuse = diff * lightColor;

	fragColor = vec4(diffuse + ambeint,1.00) * texture(albedo, outTex);
}