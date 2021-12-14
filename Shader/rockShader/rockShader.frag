#version 330 core

in VS_OUT{
	vec2 Texcoords;
}fs_in;

struct Material{
	sampler2D texture_diffuse1;
};

uniform Material material;

out vec4 FragColor;

void main(){
	FragColor = texture(material.texture_diffuse1,fs_in.Texcoords);
}