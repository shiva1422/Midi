#version 310 es

//TODO improve this shader can simplify later
precision highp float;

vec4 setWhiteKeyColor(int TopOrDown,int Index);

in vec2 whiteKeyVerts;

in vec2 blackKeyVerts;

in vec2 noteNameVerts;

in vec2 octaveNumVerts;

//TODO uniforms to buffer
//area covering keys ,startX,startY,width,height in GL Coords
uniform vec4 keysAreaBounds;

//blackKeyTranslations,5 black keys translations
uniform float blackKeyTransLationX[5];

//all in GL coords,0-whiteKeyWidth,1-keyGapX,2 whiteKeyWidth+keyGap in GL
uniform float params[3];

//keys on-1 off-0
uniform int isKeyOn[89];//TODO 88


out vec4 colorOut;

vec4 color;

vec2 finalVerts;

int index;

flat out int colType;//flat so that the values are not interpolated use for colorouti

flat out int keyNo;

flat out int topOrDown;

void main()
{
    colType = 55;//TODO ? for defaultColor?

    index = gl_InstanceID;

    if(index < 52)
    { //whiteKeys
        // TODO Maybe passing translates for each key is better than caculating from index and params

        //vertex.x + translate ;//where translate = index *(whitekeyWidthGL + keyGapGL)
        finalVerts.x = whiteKeyVerts.x + ( float(index) * ( params[2]) );//TODO this get directly params[0] + params[1]
        finalVerts.y = whiteKeyVerts.y;

        if(isKeyOn[index] == 1)
        {
            //key is on so color different
            color = vec4(0.8,0.0,0.5,1.0);////similar to below 1111
            topOrDown = 1;
        }
        else
        {
            color = vec4(1.0,1.0,1.0,1.0);
            topOrDown = 0;
        }
        if(finalVerts.y >= (keysAreaBounds.y - 0.5 * keysAreaBounds.w))//modify -0.5 to get right shade
        {
            color = vec4(0.8,0.0,0.5,1.0);//similar to above 1111//TODO no need
            color = setWhiteKeyColor(0,index);
            //
        }
        keyNo=index;
        colType = 0;
    }

    else if(index < 88)
    {//blackKeys
        finalVerts.x = blackKeyVerts.x+ ( (float((index-52)/5) * 7.0 * (params[2]) + blackKeyTransLationX[(index-52)%5] ) );
        finalVerts.y = blackKeyVerts.y;

        if(isKeyOn[index] ==1 )//TODO check with array size
        {//cuz the starting key g#(52) is ignored //so 53 is start for indices wher 52 was for draing instance
            //  color.x=1.0,color.y=1.0,color.z=0.0,color.w=1.0;
            color = vec4(0.5,0.5,0.5,1.0);//TODO make this keypress colors dynamic
        }
        else
        {
            color.x = 0.0,color.y = 0.0,color.z = 0.0,color.w = 1.0;
        }
        if(finalVerts.y >= (keysAreaBounds.y - 0.2 * keysAreaBounds.w))//x = startY, w =height in GL,modify 0.2 to get required shade
        {
            color.x = 0.5 ,color.y = 0.5,color.z = 0.5,color.w = 1.0;

        }
        colType = 0;
    }

    gl_PointSize = 2.0;//TODO

    colorOut = color;

 //   textCoodsOut = textCoods;

    gl_Position = vec4(finalVerts,1.0,1.0);

}
vec4 setWhiteKeyColor(int TopOrDown,int Index)
{
    //Rainbow colors //TODO make dymanic ?
    int x;
    vec4 whiteKeyColor;
    if(TopOrDown == 0)
    {
        x = Index%7;
    }
    else
    {
        return vec4(1.0,1.0,1.0,1.0);
    }
    switch(x)
    {
        case 0:
        {
           return vec4(148.0/255.0,0.0,211.0/255.0,1.0);
        }
        break;
        case 1:
        {
           return vec4(75.0/255.0,0.0,130.0/255.0,1.0);

        }
        break;
        case 2:
        {
           return vec4(0.0,0.0,1.0,1.0);

        }
        break;
        case 3:
        {
            return vec4(0.0,1.0,0.0,1.0);

        }
        break;
        case 4:
        {
           return vec4(1.0,1.0,0.0,1.0);

        }
        break;
        case 5:
        {
           return vec4(1.0,0.5,0.0,1.0);
        }
        break;
        case 6:
        {
            return vec4(1.0,0.0,0.0,1.0);
        }
        break;
        default:

        return vec4(1.0,1.0,1.0,1.0);

    }
}