#version 330 core
layout (location = 0) in vec3 inpPos;

uniform mat4 MVP;
uniform mat4 model;

out vec3 fragPos;
out vec3 fragNormal;

void main()
{
	fragPos = vec3(model *vec4(inpPos, 1.0));
	fragNormal = inpPos;
	gl_Position = MVP *vec4(inpPos, 1.0);
}