R"(
varying vec2 interp_tc;
attribute vec4 aPos;
attribute vec2 aTexCoord;
void main() {
    interp_tc = aTexCoord;
    gl_Position = aPos;
};
)"