#version 310 es

precision mediump float;

in vec2 verts;

void main() {
    gl_Position = vec4(verts,1.0, 1.0);
}
