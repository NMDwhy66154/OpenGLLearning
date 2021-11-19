#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main(){
	float ratio = 1.00/1.52;
	vec3 I = normalize(Position - cameraPos);
	vec3 reflectDir = reflect(I,normalize(Normal));
	vec3 refractDir = refract(I,normalize(Normal),ratio);
	FragColor = vec4(texture(skybox,reflectDir).rgb*0.8 + texture(skybox,refractDir).rgb*0.2,1);
}