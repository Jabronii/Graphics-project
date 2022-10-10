#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 scrDimensions;
uniform vec2 lightPos;
uniform float lightRadius;

in vec2 circlePos[];
in float circleRadius[];

float cross2d(vec2 v, vec2 w)
{
return v.x*w.y-v.y*w.x;
}

vec2 intersectionOfTwoLines(vec2 point1, vec2 dir1, vec2 point2, vec2 dir2)
{
vec2 A = point1;
vec2 B = point1+dir1;
vec2 C = point2;
vec2 D = point2 + dir2;
float r = ((A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y))/((B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x));
return A+r*dir1;
}

void main() {   
    vec4 position = gl_in[0].gl_Position;
    float r1 = lightRadius;
    float r2 = circleRadius[0];
    vec2 C1 = lightPos;
    vec2 C2 = circlePos[0];
    vec2 C21 = C2-C1;
    float d2 = dot(C21,C21);
    float r21 = (r2-r1)/d2;
    float s21 = sqrt(d2-(r2-r1)*(r2-r1))/d2;
    vec2 u1 = vec2(-C21.x*r21-C21.y*s21,-C21.y*r21+C21.x*s21);
    vec2 u2 = vec2(-C21.x*r21+C21.y*s21,-C21.y*r21-C21.x*s21);
    //world space
    vec2 L1 = C1+r1*u1;
    vec2 L2 = C2+r2*u1;
    vec2 R1 = C1+r1*u2;
    vec2 R2 = C2+r2*u2;
    //screen space
    L1/= scrDimensions/2.;
    L2/= scrDimensions/2.;
    R1/= scrDimensions/2.;
    R2/= scrDimensions/2.;
    //final points
    vec2 intersection = intersectionOfTwoLines(L2, L2-L1, R2, R2-R1);
    vec2 VL1 = L2;
    vec2 VR1 = R2;
    vec2 VL2 = step(r1,r2)*(((L2-L1)/length(L2-L1))*2.+L2)+step(r2,r1)*intersection;
    vec2 VR2 = step(r1,r2)*(((R2-R1)/length(R2-R1))*2.+R2)+step(r2,r1)*intersection;
    gl_Position = vec4(VL1, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(VR1, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(VL2, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(VR2, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();
    /*
    vec2 A1 = circlePos[0]+vec2( circleRadius[0],  circleRadius[0]);
    vec2 A2 = circlePos[0]+vec2(-circleRadius[0],  circleRadius[0]);
    vec2 A3 = circlePos[0]+vec2( circleRadius[0], -circleRadius[0]);
    vec2 A4 = circlePos[0]+vec2(-circleRadius[0], -circleRadius[0]);

    A1/= scrDimensions/2.;
    A2/= scrDimensions/2.;
    A3/= scrDimensions/2.;
    A4/= scrDimensions/2.;
    gl_Position = vec4(A1, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(A2, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(A3, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(A4, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();
    //p1 is the circle of smaller radius
    /*
    float r1 = min(circleRadius[0],lightRadius);
    float r2 = max(circleRadius[0],lightRadius);

    vec2 P1 = step(circleRadius[0],lightRadius)*circlePos[0] +step(lightRadius,circleRadius[0])*lightPos; 
    vec2 P2 = step(circleRadius[0],lightRadius)*lightPos +step(lightRadius,circleRadius[0])*circlePos[0]; 

    vec2 vP2P1 = P1-P2;
    vec2 I = P2+vP2P1*r2/(r2-r1);
    float sIP1 = length(vP2P1*r1/(r2-r1));
    float sIL1 = sqrt(sIP1*sIP1 - r1*r1);
    float sOL1 = sIL1*r1/sIP1;
    float sOP1 = sqrt(r1*r1-sOL1*sOL1);
    float sP2P1 = length(vP2P1);
    vec2 O = P1+vP2P1/sP2P1*sOP1;
    //raw means they might yet be flipped
    vec2 rawL1 = O+vec2(vP2P1.y, -vP2P1.x)/length(vP2P1)*sOL1;
    vec2 rawR1 = O+vec2(-vP2P1.y, vP2P1.x)/length(vP2P1)*sOL1;
    vec2 rawL2 = rawL1 + (rawL1 - I)/sIP1*(sIP1+sP2P1);
    vec2 rawR2 = rawR1 + (rawR1 - I)/sIP1*(sIP1+sP2P1);

    //world space
    vec2 L1 = step(circleRadius[0],lightRadius)*rawR2 +step(lightRadius,circleRadius[0])*rawL1;
    vec2 L2 = step(circleRadius[0],lightRadius)*rawR1 +step(lightRadius,circleRadius[0])*rawL2;
    vec2 R1 = step(circleRadius[0],lightRadius)*rawL2 +step(lightRadius,circleRadius[0])*rawR1;
    vec2 R2 = step(circleRadius[0],lightRadius)*rawL1 +step(lightRadius,circleRadius[0])*rawR2;
    //screen space
    L1/= scrDimensions/2.;
    L2/= scrDimensions/2.;
    R1/= scrDimensions/2.;
    R2/= scrDimensions/2.;
    //final points
    vec2 intersection = intersectionOfTwoLines(L2, L2-L1, R2, R2-R1);
    vec2 VL1 = L2;
    vec2 VR1 = R2;
    vec2 VL2 = step(lightRadius,circleRadius[0])*(L2+(L2-L1)/length(L2-L1)*2.)+step(circleRadius[0],lightRadius)*intersection;
    vec2 VR2 = step(lightRadius,circleRadius[0])*(R2+(R2-R1)/length(R2-R1)*2.)+step(circleRadius[0],lightRadius)*intersection;
    //vec2 VL2 = step(lightRadius,circleRadius[0])*(L2+(L2-L1)/length(L2-L1)*2.)+step(circleRadius[0],lightRadius)*(VL1+VR1)/2.;
    //vec2 VR2 = step(lightRadius,circleRadius[0])*(R2+(R2-R1)/length(R2-R1)*2.)+step(circleRadius[0],lightRadius)*(VL1+VR1)/2.;
    //vec2 VL2 = step(lightRadius,circleRadius[0])*vec2(0.)+step(circleRadius[0],lightRadius)*(VL1+VR1)/2.;
    //vec2 VR2 = step(lightRadius,circleRadius[0])*vec2(0.)+step(circleRadius[0],lightRadius)*(VL1+VR1)/2.;
    gl_Position = vec4(VL1, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(VR1, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(VL2, 0.0, 1.0);
    EmitVertex();  
    gl_Position = vec4(VR2, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();
    */
}