R"(
varying vec2 interp_tc;
attribute vec4 in_pos;
attribute vec2 in_tc;
void main() {
    interp_tc = in_tc;
    gl_Position = in_pos;
};
)"