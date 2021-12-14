#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};



out VS_OUT{
	out vec3 FragPos;
	out vec3 Normal;
	out vec2 TexCoords;
	out vec3 ClipSpaceNormal;
} vs_out;

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos,1.0);
	vs_out.FragPos = vec3(model * vec4(aPos,1.0));
	vs_out.Normal = mat3(transpose(inverse(model)))*aNormal;
	vs_out.TexCoords = aTexCoords;
	mat3 nomralMatrix = mat3(transpose(inverse(view * model)));
	vs_out.ClipSpaceNormal = normalize(vec3(projection * vec4(nomralMatrix*aNormal,0.0)));
}