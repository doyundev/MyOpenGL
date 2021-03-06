#version 330 core
out vec4 FragColor;
in vec3 TintColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixRate;

void main()
{
	//FragColor = texture(texture1, TexCoord) * vec4(TintColor, 1.0f);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixRate);
}