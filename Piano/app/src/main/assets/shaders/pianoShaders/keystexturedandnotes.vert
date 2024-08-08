#version 310 es

//TODO improve this shader can simplify later
precision highp float;

vec4 setWhiteKeyColor(int TopOrDown,int Index);

in vec2 whiteKeyVerts;

in vec2 blackKeyVerts;

in vec2 noteNameVerts;

in vec2 octaveNumVerts;

in vec2 textureCoords;

//TODO uniforms to buffer
//area covering keys ,startX,startY,width,height in GL Coords
uniform vec4 keysAreaBounds;

//blackKeyTranslations,5 black keys translations
uniform float blackKeyTransLationX[5];

//all in GL coords,0-whiteKeyWidth,1-keyGapX,2 whiteKeyWidth+keyGap in GL
uniform float params[3];

//keys on-1 off-0
uniform int isKeyOn[89];//TODO 88

vec2 finalVerts;

flat out int bKeyOn;

flat out int bWhiteKey;

out vec2 textCoordsOut;

void main()
{
    int index = gl_InstanceID;

    if(index < 52)
    { //whiteKeys
        // TODO Maybe passing translates for each key is better than caculating from index and params

        //vertex.x + translate ;//where translate = index *(whitekeyWidthGL + keyGapGL)
        finalVerts.x = whiteKeyVerts.x + ( float(index) * ( params[2]) );//TODO this get directly params[0] + params[1]
        finalVerts.y = whiteKeyVerts.y;

        if(isKeyOn[index] == 1)
        {
            bKeyOn = 1;//direct equal instead of equals to check
        }
        else
        {
            //color = vec4(1.0,1.0,1.0,1.0);
            bKeyOn = 0;
        }

        bWhiteKey = 1;
    }

    else if(index < 88)
    {//blackKeys
        finalVerts.x = blackKeyVerts.x+ ( (float((index-52)/5) * 7.0 * (params[2]) + blackKeyTransLationX[(index-52)%5] ) );
        finalVerts.y = blackKeyVerts.y;

        if(isKeyOn[index] ==1 )//TODO check with array size
        {
            bKeyOn = 1;
            finalVerts.y -= 0.01;
        }
        else
        {

            bKeyOn = 0;
        }
        bWhiteKey = 0;
    }

    //gl_PointSize = 2.0;//TODO

    textCoordsOut = textureCoords;

    gl_Position = vec4(finalVerts,1.0,1.0);

}

