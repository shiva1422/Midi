#version 310 es

//PianoView.CPP
//TODO improve this shader can simplify later combine verts and use ubo.
//TODO draw note names in separate draw and check if conditional performance
precision mediump float;

in vec2 whiteKeyVerts;

in vec2 blackKeyVerts;

in vec2 noteNameVerts;

in vec2 octaveNumVerts;

in vec2 allKeysVerts;

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

flat out int keyNum;

flat out int keyType;//0 blackkey,1 whitekey,2 allkeys,3 noteNames,4 octaveNum;

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

        keyType = 1;
    }

    else if(index < 88)
    {//blackKeys
        finalVerts.x = blackKeyVerts.x + ( (float((index-52)/5) * 7.0 * (params[2]) + blackKeyTransLationX[(index-52)%5] ) );
        finalVerts.y = blackKeyVerts.y;

        if(isKeyOn[index] ==1 )//TODO check with array size
        {
            bKeyOn = 1;
            finalVerts.y -= 0.005;
        }
        else
        {

            bKeyOn = 0;
        }
        keyType = 0;
    }
    else if(index == 88)
    {
        finalVerts = allKeysVerts;
        keyType = 2;
    }
    else//note name,octavenumber
    {
        if(index<141)//
        {
             finalVerts.x = noteNameVerts.x + (float(index - 89) * (params[0] + params[1]));
             finalVerts.y = noteNameVerts.y;
             keyType = 3;
             keyNum = ( 52 - (141 - index))%7;//A to g ,0 - 7

        }
        else
        {
           // keyNo=index-144;
            finalVerts.x = octaveNumVerts.x + (float(index - 141) * (params[0] + params[1]));
            finalVerts.y = octaveNumVerts.y;
            keyType = 4;
            keyNum = (index - 136)/7 + 7;

        }

    }
    //gl_PointSize = 2.0;//TODO

    textCoordsOut = textureCoords;

    gl_Position = vec4(finalVerts,1.0,1.0);

}

