#version 330 core
out vec4 FragColor;

in vec3 circleColor;
in float radius;
in vec3 centrePos;
in float excitement;
in float isLight;

uniform float time;

in vec3 fragPos;
float myLength(vec3 vector)
{
	return sqrt(vector.x*vector.x+vector.y*vector.y);
}
void main()
{
	//float timeCycle = (sin(time)+1.)/2.;
	float distanceToCenter = length(fragPos-centrePos);
	//vec3 toCenter = fragPos-centrePos;
	//toCenter/=50.;
	//distanceToCenter/= 20.;
	//distanceToCenter = fract(distanceToCenter);
	float isInCircle = step(distanceToCenter,radius);
	//FragColor = vec4(circleColor + vec3(excitement, (sin(time+distanceToCenter/15.+ excitement)+1.)/2.,0.), isInCircle);
	//FragColor = vec4(circleColor, isInCircle);
	//FragColor = vec4(step(distanceToCenter,200.))+vec4(step(length(centrePos), 200.));
	//float absxOryGreaterThan1 = 1.-step(abs(toCenter.x),1.)*step(abs(toCenter.y),1.);

	//FragColor = mix(vec4(abs(toCenter.x),abs(toCenter.y),0.,1.), vec4(0.0,0.0,0.0,1.), absxOryGreaterThan1);
	//FragColor = vec4(vec3(step(myLength(toCenter), 50.)),1.);

	//FragColor = vec4(vec3(step(abs(toCenter.x)+abs(toCenter.y), 50.)),1.);
	FragColor = isInCircle*vec4(circleColor,1.);
	FragColor = mix(FragColor,vec4(1.)*FragColor[3], isLight);
	//float rad = 40.;
	//FragColor = vec4(step(abs(fragPos.x-centrePos.x), rad))*vec4(step(abs(fragPos.y-centrePos.y), rad));
}