#shader vertex
#version 330
uniform mat4 projMat;
uniform mat4 viewMat;
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
out vec4 vertexColor;
void main()
{
	gl_Position =  projMat * vec4(position, 1);
	vertexColor = color;
	vertexColor = vec4(1, 0.0, 0.0, 1.0);
	gl_PointSize = 3;	// point size is only used for the velocity curve
};

#shader fragment
#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
void main()
{
	FragColor = vertexColor;
};