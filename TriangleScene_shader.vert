#version 330 core
layout (location = 0) in vec3 Pos;

out vec3 FragPos;

void main()
{
	FragPos = Pos;
	gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);
}
/*
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
*/