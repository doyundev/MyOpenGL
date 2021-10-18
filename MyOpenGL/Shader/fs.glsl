#version 330 core
out vec4 FragColor;
uniform vec3 tintColor;
in vec3 ourColor;

void main()
{
	FragColor = vec4(tintColor.x, tintColor.y,tintColor.z,1.0f);
}