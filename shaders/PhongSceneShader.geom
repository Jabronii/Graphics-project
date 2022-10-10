#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 MVP;
uniform mat4 model;

out vec3 fragPos;
out vec3 fragNormal;


void main()
{
	vec3 side1 = (model * (gl_in[0].gl_Position-gl_in[1].gl_Position)).xyz;
	vec3 side2 = (model * (gl_in[2].gl_Position-gl_in[1].gl_Position)).xyz;
	vec3 triangleNormal = normalize(cross(side1, side2));
	

	gl_Position = MVP * gl_in[0].gl_Position;
	fragPos = vec3(model * gl_in[0].gl_Position);
	fragNormal = triangleNormal;
	EmitVertex();
	gl_Position = MVP * gl_in[1].gl_Position;
	fragPos = vec3(model * gl_in[1].gl_Position);
	fragNormal = triangleNormal;
	EmitVertex();
	gl_Position = MVP * gl_in[2].gl_Position;
	fragPos = vec3(model * gl_in[2].gl_Position);
	fragNormal = triangleNormal;
	EmitVertex();
	EndPrimitive();
}