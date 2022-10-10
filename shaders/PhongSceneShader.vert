#version 330 core
layout (location = 0) in vec3 inpPos;

void main()
{
	gl_Position = vec4(inpPos, 1.0);
}