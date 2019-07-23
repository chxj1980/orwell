#version 130

attribute vec4 vertexIn;
attribute vec2 textureIn;
varying vec2 textureOut;
void main(void)
{
	gl_Position = vertexIn;
	textureOut = vec2(textureIn.x, 1.0 - textureIn.y);
}