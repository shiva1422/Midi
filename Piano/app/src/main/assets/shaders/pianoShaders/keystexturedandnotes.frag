#version 310 es

precision mediump float;

precision mediump sampler2DArray;

in vec4 colorOut;

//in vec2 textCoodsOut;

flat in int colType;

flat in int bKeyOn;

flat in int bWhiteKey;

in vec2 textCoordsOut;

out vec4 Fragcolor;

uniform sampler2D keyTextures[4];//white off,on,black off, on

void main()
{
    if(bWhiteKey == 1)
    {
        //Fragcolor = vec4(1.0,1.0,1.0,1.0);
        if(bKeyOn == 0)
        Fragcolor = texture(keyTextures[0], textCoordsOut);
        else
        Fragcolor = texture(keyTextures[1], textCoordsOut);

    }
    else
    {
        if(bKeyOn == 0)
        Fragcolor = texture(keyTextures[2], textCoordsOut);
        else
        Fragcolor = texture(keyTextures[3], textCoordsOut);

    }

}
