R"(
#version 330 core
#extension GL_OES_EGL_image_external : require
precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;
uniform samplerExternalOES tex;

void main()
{
	FragColor = texture2D(tex, TexCoord);
}
)"