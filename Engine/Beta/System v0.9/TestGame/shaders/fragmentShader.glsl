#version 430

in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentPosition;
in vec3 lightVector[4];

out vec4 color;

uniform vec3 cameraPos;
uniform vec3 lightAttenuation[4];

void main()
{
	vec3 colorLights = vec3(0,0,0);
	vec3 colorLights2 = vec3(1,1,1);
	float reflectivity = 0.0;
	vec3 lightColor[4];
	lightColor[0] = colorLights2;
	lightColor[1] = colorLights;
	lightColor[2] = colorLights;
	lightColor[3] = colorLights;
	vec3 cameraVec = normalize(cameraPos - fragmentPosition);


	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);
	for(int i = 0; i < 4; i++)
	{
	//attenuation
	float lightDistance = length(lightVector[i]);
	float attenuationFactor = lightAttenuation[i].x + (lightAttenuation[i].y * lightDistance) + (lightAttenuation[i].z * lightDistance * lightDistance);

	//Diffuse
	vec3 lightVec = normalize(lightVector[i]);
	float brightness = max(dot(lightVec, fragmentNormal),0.0);
	vec3 diffuseLight = (brightness * lightColor[i]) / attenuationFactor;
	totalDiffuse = totalDiffuse + diffuseLight;

	//Specular
	vec3 reflecLightVec = reflect(-lightVec, fragmentNormal);
	float specularity = max( dot(reflecLightVec, cameraVec),0.0);
	specularity = pow(specularity, 50);
	vec3 specularLight = (specularity * reflectivity * lightColor[i]) / attenuationFactor;
	totalSpecular = totalSpecular + specularLight;

	}
	totalDiffuse = max(totalDiffuse, 0.2);

	color = fragmentColor * vec4(totalDiffuse, 1) + vec4(totalSpecular, 1);
}