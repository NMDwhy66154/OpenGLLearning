#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void GenerateLine()
{
    vec4 center = vec4(0.0,0.0,0.0,0.0);
    for(int i = 0 ; i<3;i++)
    {
        center = center + gl_in[i].gl_Position;
    }
    center = vec4(center.x/3,center.y/3,center.z/3,center.w/3);
    gl_Position = center;
    EmitVertex();
    center = vec4(0.0,0.0,0.0,0.0);
    for(int i = 0 ; i<3;i++)
    {
        center = center + gl_in[i].gl_Position + vec4(gs_in[i].normal, 0.0) * MAGNITUDE;
    }
    center = vec4(center.x/3,center.y/3,center.z/3,center.w/3);
    gl_Position = center;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    //GenerateLine(0); // 第一个顶点法线
    //GenerateLine(1); // 第二个顶点法线
    //GenerateLine(2); // 第三个顶点法线
    GenerateLine();
}