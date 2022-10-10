#version 330 core
layout (location = 0) in vec3 inpPos;
layout (location = 1) in mat4 MVP;
layout (location = 5) in mat4 model;
layout (location = 9) in float inpRadius;
layout (location = 10) in vec3 inpCentrePos;
layout (location = 11) in float inpExcitement;
layout (location = 12) in vec3 inpCircleColor;
layout (location = 13) in float inpIsLight;

out vec3 circleColor;
out float radius;
out vec3 centrePos;
out float excitement;
out float isLight;

out vec3 fragPos;

out vec3 testVar;

void main()
{
	circleColor = inpCircleColor;
	radius = inpRadius;
	//radius = 15.;
	centrePos = inpCentrePos;
	excitement = inpExcitement;
	isLight = inpIsLight;

	//fragPos = (vec4(inpPos, 1.0)/2.+1.).xyz;
	fragPos = (model * vec4(inpPos,1.)).xyz;
	fragPos.z = 0.0;
	gl_Position = MVP * vec4(inpPos, 1.0);
	gl_Position = vec4(gl_Position.x, gl_Position.y, 0., gl_Position.w);
}