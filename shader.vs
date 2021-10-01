#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float uniColor;
uniform float xOffset;
out vec3 ourColor;
void main()
{
	vec3 offset = aPos +vec3(xOffset-0.5,0,0);
	gl_Position = vec4(-1*offset,1.0);
	ourColor = vec3(uniColor,uniColor,uniColor);
	ourColor = ourColor * aColor;
	ourColor = aPos;
}