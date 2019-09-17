R"(
varying vec2 textureOut;
uniform sampler2D tex;
void main() {
    //vec3 rgb;
    //rgb.x = texture2D(tex, textureOut).r;
    gl_FragColor = texture2D(tex, textureOut);
    //gl_FragColor = texture2D(tex, textureOut);
    //gl_FragColor = vec4(1.0, 0, 0, 1.0);
};
)"