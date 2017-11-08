#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inputColor;

out vec3 TexCoords;
out vec3 fragmentColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
	mat3 noTranslateview = mat3(view);
    gl_Position =   projection * mat4(noTranslateview) * model * vec4(position, 1.0);
    TexCoords = position;
	fragmentColor = inputColor;
} 