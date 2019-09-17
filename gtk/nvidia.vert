R"(
varying vec2 textureOut;
attribute vec4 aPos;
attribute vec2 aTexCoord;
void main() {
    textureOut = aTexCoord;
    gl_Position = aPos;
};
)"