#version 310 es

//TODO improve this shader can simplify later
precision mediump float;

in vec2 verts;

in vec2 textureCoords;

out vec2 textCoordsOut;

flat out int isBlock;
//
out vec2 vertsOut;

void main()
{

    textCoordsOut = textureCoords;

    vertsOut = verts;

    gl_Position = vec4(verts, 1.0, 1.0);
}