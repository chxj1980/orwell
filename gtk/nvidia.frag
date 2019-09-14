R"(
#extension GL_OES_EGL_image_external : require
precision mediump float;
varying vec2 interp_tc;
uniform samplerExternalOES tex;
void main() {
    gl_FragColor = vec4(1.0,0,0,1.0);
};
)"