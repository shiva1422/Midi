#version 310 es

precision mediump float;

precision mediump sampler2DArray;

in vec4 colorOut;

//in vec2 textCoodsOut;

flat in int colType;

flat in int keyNo;//0-52 for white keys

flat in int topOrDown;//top1 down0;

out vec4 Fragcolor;

layout(location =125)uniform sampler2D tex;

layout(location =126)uniform sampler2DArray noteNamesTex;

void main()
{

    if(colType == 0)
    {
        Fragcolor = colorOut;
        // Fragcolor=setWhiteKeyColor(topOrDown,keyNo);
        // Fragcolor= texture(tex, textCoodsOut)*colorOut.bgar;
    }
    /*
    switch(colType)
    {
        case 0://whiteKeys
        {
            Fragcolor = colorOut;
            // Fragcolor=setWhiteKeyColor(topOrDown,keyNo);

            // Fragcolor= texture(tex, textCoodsOut)*colorOut.bgar;
        }break;
        case 1://blackKeys
        {
            Fragcolor = colorOut;
        }break;
        /*
        case 5:
        {
            // vec4 finalColor= texture(tex, textCoodsOut);//use this for drawing text with background color and text color
            //Fragcolor=finalColor;
            Fragcolor=colorOut;

        }break;*/
       /* case 6:
        {//notenames sampler2darray noteNamesTexture
            int keyName=(keyNo)%7;
            vec4 finalColor= texture(noteNamesTex, vec3(textCoodsOut,keyName))*colorOut;
            if(finalColor.a<0.5)
            discard;
            Fragcolor=finalColor;

        }
        break;
        case 7:
        {
            //octaveNums
            int keyName=(keyNo+5)/7+7;
            vec4 finalColor= texture(noteNamesTex, vec3(textCoodsOut,keyName))*colorOut;
            if(finalColor.a<0.5)
            discard;
            Fragcolor=finalColor;
        }
        break;
        default:
        {
            Fragcolor=vec4(1.0,0.0,0.0,1.0);
        }
    }*/


}
