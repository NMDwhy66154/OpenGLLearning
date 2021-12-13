#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;



void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0);
	FragPos = vec3(model * vec4(aPos,1.0));
	gl_PointSize = gl_Position.z;
	Normal = mat3(transpose(inverse(model)))*aNormal;
	TexCoords = aTexCoords;
}