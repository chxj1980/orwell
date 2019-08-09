#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 ourColor;

uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main(void)
{
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture(tex_y, TexCoord).r;
	yuv.y = texture(tex_u, TexCoord).r - 0.5;
	yuv.z = texture(tex_v, TexCoord).r - 0.5;
	rgb = mat3(1.0, 1.0, 1.0,
		0.0, -0.39465, 2.03211,
		1.13983, -0.58060, 0.0) * yuv;
	FragColor = vec4(rgb, 1.0);
	//FragColor = vec4(1.0,0,0, 1.0);
}
