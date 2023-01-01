#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

vec4 vertexColor;

void main()
{
	gl_Position = position;
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vertexColor;

void main()
{
	color = vertexColor;
}