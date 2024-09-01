#version 310 es

precision mediump float;

precision mediump sampler2DArray;

in vec4 colorOut;

//in vec2 textCoodsOut;

flat in int colType;

flat in int bKeyOn;

flat in int keyNum;

flat in int keyType;

in vec2 textCoordsOut;

out vec4 Fragcolor;

uniform sampler2D keyTextures[4];//white off,on,black off, on

uniform vec4 keyNameColor;
uniform sampler2DArray notesTextures;

uniform sampler2D allKeysTexture;

void main()
{
//TODO check if key and note names can be draw at once using mix option//
    vec4 col;
    vec4 color = vec4(0.0,0.0,0.0,1.0);//TODO dynamically set
    switch(keyType)
    {
        case 0 ://blackKey
        if (bKeyOn == 0)
        Fragcolor = texture(keyTextures[2], textCoordsOut);
        else
        Fragcolor = texture(keyTextures[3], textCoordsOut);
        break;

        case 1 ://whitekey
        //Fragcolor = vec4(1.0,1.0,1.0,1.0);
        if (bKeyOn == 0)
        Fragcolor = texture(keyTextures[0], textCoordsOut);
        else
        {
            Fragcolor = texture(keyTextures[1], textCoordsOut);;
        }
        break;

        case 2: //allkeys
        Fragcolor = texture(allKeysTexture,textCoordsOut);
        break;

        case 3://note name
        col = texture(notesTextures, vec3(textCoordsOut.xy,keyNum));
        if(col.x <= 0.2)
        discard;
        Fragcolor = color;
        break;

        case 4 :// octave num
        col = texture(notesTextures, vec3(textCoordsOut.xy,keyNum));
        if(col.x <= 0.2)
        discard;
        Fragcolor = color;
        break;

        default :
        Fragcolor = vec4(1.0, 1.0, 0.0, 1.0);

    }


}
