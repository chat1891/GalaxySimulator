#shader vertex
#version 440 core
#define DEG_TO_RAD 0.01745329251

uniform mat4 projMat;
uniform mat4 viewMat;
uniform int pertN;
uniform int dustSize;
uniform int displayFeatures;
uniform float pertAmp;
uniform float time;

layout(location = 0) in float theta0;
layout(location = 1) in float velTheta;
layout(location = 2) in float tiltAngle;
layout(location = 3) in float a;
layout(location = 4) in float b;
layout(location = 5) in int type;
layout(location = 6) in float eccentricity;
layout(location = 7) in vec4 color;

out vec4 vertexColor;
flat out int vertexType;
flat out int features;

vec2 calcPos(float a, float b, float theta, float velTheta, float time, float tiltAngle) {
	float curTheta= theta + velTheta * time;
	float beta = -tiltAngle;
	float alpha = curTheta * DEG_TO_RAD;
	float cosalpha = cos(alpha);
	float sinalpha = sin(alpha);
	float cosbeta = cos(beta);
	float sinbeta = sin(beta);
	vec2 center = vec2(0,0);
	vec2 ps = vec2(center.x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta),
			       center.y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta));

	ps.x += (a / pertAmp) * sin(alpha * 2.0 * pertN);
	ps.y += (a / pertAmp) * cos(alpha * 2.0 * pertN);

	return ps;
}

void main()
{
	vec2 ps = calcPos(a, b, theta0, velTheta, time, tiltAngle);

	if (type==0) {
		gl_PointSize = 2.0;
		vertexColor = color;
	} 
	gl_Position =  projMat * vec4(ps, 0, 1);
    gl_PointSize = max(gl_PointSize, 0.0);
	vertexType = type;
	features = displayFeatures;
}


#shader fragment
#version 440 core
in vec4 vertexColor;
flat in int vertexType;
flat in int features;
out vec4 FragColor;
void main()
{
	if ((features & 1) == 0)
		discard;
	vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
	float alpha = 1 - length(circCoord);
	FragColor = vec4(vertexColor.xyz, alpha);

}