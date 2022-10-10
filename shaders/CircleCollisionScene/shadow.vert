#version 330 core
layout (location = 0) in vec3 inpPos;
layout (location = 1) in float inpRadius;

uniform vec2 scrDimensions;
uniform vec2 lightPos;

out vec2 circlePos;
out float circleRadius;

void main()
{
    circlePos = inpPos.xy;
    circleRadius = inpRadius;
    gl_Position = vec4(inpPos.x, inpPos.y, 0.0, 1.0)/vec4(scrDimensions/2., 1.,1.); 
    //gl_Position = vec4(lightPos.x, lightPos.y, 0.0, 1.0);
}