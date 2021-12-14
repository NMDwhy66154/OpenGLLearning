#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
    vec2 TexCoords;
    vec3 ClipSpaceNormal;
} gs_in[];


out GE_OUT{
    vec3 FragPos;
	vec3 Normal;
    vec2 TexCoords;
} ge_out;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.1;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main(){
    
    for(int i = 0 ; i<3;i++){
        vec3 normal = gs_in[i].ClipSpaceNormal;
        gl_Position = explode(gl_in[i].gl_Position,normal);
        ge_out.TexCoords = gs_in[i].TexCoords;
        ge_out.Normal = gs_in[i].Normal;
        ge_out.FragPos = gs_in[i].FragPos;
        EmitVertex();
    }
    EndPrimitive();
}