#version 310 es

precision mediump float;

precision mediump sampler2DArray;

in vec2 textCoordsOut;

out vec4 Fragcolor;

uniform sampler2D texture1;

void main()
{

    vec4 col = texture(texture1, textCoordsOut);
    col.a = 0.5;
    Fragcolor = col;

}