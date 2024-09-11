#version 310 es

precision mediump float;

precision mediump sampler2DArray;

in vec2 textCoordsOut;

flat in  int isBlock;

out vec4 Fragcolor;

uniform vec4 blockParams;//first one for bounds ,second for color;

in vec2 vertsOut;

uniform sampler2D texture1;

void main()
{
       if(vertsOut.x >= blockParams.x && vertsOut.x <= blockParams.z && vertsOut.y <= blockParams.y && vertsOut.y >= blockParams.w)
       {
           Fragcolor =  texture(texture1, textCoordsOut);
       }else
       Fragcolor = texture(texture1, textCoordsOut)* 0.5;
}