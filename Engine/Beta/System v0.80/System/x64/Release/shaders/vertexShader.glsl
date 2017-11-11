#version 430

in layout(location=0) vec4 position;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 normalVec;

uniform mat4 fullTransformMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition[4];

out vec4 fragmentColor;
out vec3 fragmentNormal;
out vec3 fragmentPosition;
out vec3 lightVector[4];

void main()
{
	gl_Position = fullTransformMatrix * position;
	fragmentColor = vertexColor;
	fragmentNormal = mat3(modelMatrix) * normalVec;
	fragmentPosition = vec3(modelMatrix * position);
	for(int i = 0; i < 4; i++)
	{
		lightVector[i] = lightPosition[i] - fragmentPosition;
	}
}