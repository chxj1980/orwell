R"(
#version 330 core
#extension GL_OES_EGL_image_external : require
precision mediump float;
in vec2 TexCoord;
uniform samplerExternalOES tex;
out vec4 FragColor;

void main()
{
    FragColor = texture2D(tex, TexCoord);
    //FragColor = vec4(1.0f, 0, 0, 1.0f);
}
)"