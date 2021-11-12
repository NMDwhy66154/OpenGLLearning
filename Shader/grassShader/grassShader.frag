#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D grass;
void main()
{
    vec4 texColor = texture(grass,TexCoords);
    //if(texColor.a<0.1)
    //    discard;
    FragColor = texColor; 
    //FragColor = vec4(vec3(TexCoords.x),1.0);
}