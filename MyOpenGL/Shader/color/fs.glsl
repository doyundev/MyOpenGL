#version 330 core
out vec4 FragColor;
uniform vec3 tintColor;
in vec3 ourColor;
in vec3 ourPos;

void main()
{
	//FragColor = vec4(tintColor.x, tintColor.y,tintColor.z,1.0f);
	FragColor = vec4(ourPos.x, ourPos.y, ourPos.z, 1.0f);
}