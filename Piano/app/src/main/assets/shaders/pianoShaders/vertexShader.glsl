#version 310 es
precision highp float;
vec4 setWhiteKeyColor(int TopOrDown,int Index);
layout(location = 0) in vec2 whiteVertices;
layout(location = 1) in vec2 blackVertices;
layout(location = 2) in vec2 backVertices;
// "layout(location = 3) in vec2 padVertices;\n"
layout(location = 4) in vec2 keySizeAdjusters;
layout(location = 5) in vec2 keyPositionAdjuster;
layout(location = 6) in vec2 textCoods;
layout(location = 7) in vec2 allKeysBar;
layout(location = 8) in vec2 noteNameCoods;
layout(location = 9) in vec2 octaveNumCoods;
// "layout(location = 4) uniform vec4 keyOnColors[3];\n"
//TODO cache all location after creation instead of hard coded
layout(location = 1)uniform float RadianAngle;
//TODO below parmas independent of displayParams
layout(location = 5) uniform float displayParams[5];//includes screeH,w,whitekeyW,keyGap,padH
layout(location = 10) uniform float keyTranslationsX[5];//blackKeyTranslations
layout(location = 20) uniform int isKeyOn[89];//for pads and keys on-1 off-0
layout(location = 120) uniform int drawType;
layout(location = 122) uniform float keyAdjusterTranslations[2];//sizeIncTrans,postionsTrans
out vec4 colorOut;
out vec2 textCoodsOut;
vec4 color;
float finalPositionX,octaveNum;
vec3 finalVerts;
vec3 finalColor;
int index;
int subDrawType;
flat out int colType;///////flat so that the values are not interpolated use for colorouti
flat out int keyNo;
flat out int topOrDown;//top1 down0;
//  "mat2 rotation;\n"
void main()
{
    colType = 55;
    index = gl_InstanceID;
    finalVerts.z = 0.0;
    if(drawType == 0) //for complete piano set
    {
        if (index < 52)
        {
            subDrawType=0;//whiteKeys
        }
        else if (index < 88 && index>=52)
        {
            subDrawType=1;//black keys
        }
        else if (index == 88)
        {
            subDrawType=2;//background above keys
        }
        else if (index == 89)
        {
            subDrawType=3;//keySize--
        }
        else if (index == 90)
        {
            subDrawType=4;//keySize++
        }
        else if (index == 91)
        {
            subDrawType=5;//allkeybar
        }
        else if(index < 196)
        {
            subDrawType=7;//for note names(52) only
        }
    }
    else if(drawType==1)//for only keyPosition changer complete this as wee in drawtype0;
    {
        subDrawType=6;////keyPositionChanger
    }
    switch(subDrawType)
    {
        case 0:
        {
            //whitekeys
            finalVerts.x = whiteVertices.x+(float(index)*(displayParams[2]+displayParams[3])*2.0)/displayParams[0];
            finalVerts.y=whiteVertices.y;
            if(isKeyOn[index]==1)
            {
                color.x=0.8,color.y=0.0,color.z=0.5,color.w=1.0;//same as below 1111
                topOrDown=1;
            }
            else
            {
                color.x=1.0,color.y=1.0,color.z=1.0,color.w=1.0;
                topOrDown=0;
            }
            colType=0;
            if(whiteVertices.y>=-0.2)
            {
                color.x=0.8,color.y=0.0,color.z=0.5,color.w=1.0;//similar to above 1111
                color=setWhiteKeyColor(0,index);
               //

            }
            keyNo=index;

        }
        break;
        case 1:
        {//blackKeys start at 52 instance id

            finalVerts.x=blackVertices.x+(( float((index-52)/5)*7.0*(displayParams[2]+displayParams[3])+keyTranslationsX[(index-52)%5])*2.0)/displayParams[0];
            finalVerts.y=blackVertices.y;
            if(isKeyOn[index+1]==1)
            {//cuz the starting key g#(52) is ignored //so 53 is start for indices wher 52 was for draing instance
              //  color.x=1.0,color.y=1.0,color.z=0.0,color.w=1.0;
                color=vec4(0.5,0.5,0.5,1.0);
            }
            else
            {
                color.x=0.0,color.y=0.0,color.z=0.0,color.w=1.0;
            }
            if(blackVertices.y>=-0.2)
            {
                color.x=0.5,color.y=0.5,color.z=0.5,color.w=1.0;

            }
                colType=1;
        }
        break;
        case 2:
        { //backGoround abovekeys
            finalVerts.x=backVertices.x;
            finalVerts.y=backVertices.y;
            color.x=0.0, color.y=0.2, color.z=0.2, color.w=1.0;
        }
        break;
        case 3:
        {//keySize--
            finalVerts.x=keySizeAdjusters.x;
            finalVerts.y=keySizeAdjusters.y;
            color.x=0.5,color.y=0.5,color.z=0.5,color.w=0.5;
            colType=2;
        }
        break;
        case 4:
        { //keySize++
            finalVerts.x=keySizeAdjusters.x+(keyAdjusterTranslations[0]*2.0)/displayParams[0];
            finalVerts.y=keySizeAdjusters.y;
            color.x=0.5,color.y=0.5,color.z=0.5,color.w=1.0;
            colType=3;
        }
        break;
        case 5:
        {//keyPositonchanger full or allkeysbar
            finalVerts.x=allKeysBar.x;
            finalVerts.y=allKeysBar.y;
            color.x=0.0,color.y=1.0,color.z=1.0,color.w=1.0;
            colType=4;
        }
        break;
        case 6:
        {         ///keyPositionchanger
            finalVerts.x= keyPositionAdjuster.x+(( keyAdjusterTranslations[1]*2.0))/displayParams[0];
            finalVerts.y=keyPositionAdjuster.y;
            color.x=1.0,color.y=0.0,color.z=0.0,color.w=0.4;
            colType=5;
        }
        break;
        case 7:
        {////////notenames and octaveNums
            if(index<144)//
            {
                keyNo=index-92;
                finalVerts.x=noteNameCoods.x+(float(keyNo)*(displayParams[2]+displayParams[3])*2.0)/displayParams[0];;
                finalVerts.y=noteNameCoods.y;
                colType=6;

            }
            else if(index>143)//octaveNums
            {
                keyNo=index-144;
                finalVerts.x=octaveNumCoods.x+(float(keyNo)*(displayParams[2]+displayParams[3])*2.0)/displayParams[0];;
                finalVerts.y=octaveNumCoods.y;
                colType=7;
            }
             if(isKeyOn[keyNo]==1)
             {
                 color.x=1.0,color.y=1.0,color.z=1.0,color.w=1.0;
             }
             else
             {
                 color.x=0.5,color.y=0.0,color.z=0.5,color.w=1.0;
             }
        }
        break;
    }

//   "rotation=mat2(cos(RadianAngle),sin(RadianAngle),-sin(RadianAngle),cos(RadianAngle));\n "
    gl_PointSize=2.0;
    colorOut=color;
    textCoodsOut=textCoods;
    gl_Position=vec4(finalVerts.x,finalVerts.y,1,1);

}
vec4 setWhiteKeyColor(int TopOrDown,int Index)
{
    int x;
    vec4 whiteKeyColor;
    if(TopOrDown==0)
    {
        x=Index%7;
    }
    else
    {
        x=100;//random number so that difault case is execture to set color to white;
    }
    switch(x)
    {
        case 0:
        {
            whiteKeyColor=vec4(148.0/255.0,0.0,211.0/255.0,1.0);
        }
        break;
        case 1:
        {
            whiteKeyColor=vec4(75.0/255.0,0.0,130.0/255.0,1.0);

        }
        break;
        case 2:
        {
            whiteKeyColor=vec4(0.0,0.0,1.0,1.0);

        }
        break;
        case 3:
        {
            whiteKeyColor=vec4(0.0,1.0,0.0,1.0);

        }
        break;
        case 4:
        {
            whiteKeyColor=vec4(1.0,1.0,0.0,1.0);

        }
        break;
        case 5:
        {
            whiteKeyColor=vec4(1.0,0.5,0.0,1.0);
        }
        break;
        case 6:
        {
            whiteKeyColor=vec4(1.0,0.0,0.0,1.0);
        }
        break;
        default:
        return vec4(1.0,1.0,1.0,1.0);

    }
    return whiteKeyColor;
}