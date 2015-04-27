#version 130


in vec3 EntryPoint;
in vec4 ExitPointCoord;

uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;  
uniform float     StepSize;
uniform vec2      ScreenSize;
uniform float transferFunction[256*4];
uniform float ROItransferFunction[50];
uniform float ROI[6];

//layout (location = 0) 
out vec4 FragColor;


void main()
{
    vec3 exitPoint = texture(ExitPoints, gl_FragCoord.st/ScreenSize).xyz;
  
    if (EntryPoint == exitPoint)
        //background need no raycasting
        discard;
 
    vec3 dir = exitPoint - EntryPoint;
    float rayLength = length(dir); // the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir = normalize(dir) * StepSize;
    float deltaDirLen = length(deltaDir);
    vec3 voxelCoord = EntryPoint;
    vec4 colorAcum = vec4(0.0);           // The dest color
    float alphaAcum = 0.0;                // The  dest alpha for blending
    float intensity;
    float rayAccumulatedLength = 0.0;
    vec4 colorSample; // The src color 
    float alphaSample; // The src alpha
    bool flag = false;
    bool isHittingROI = false;
   
    float sampleOpacityValue = 0.0;
    float accumulatedOpacity = 0.0;

    vec4 bgColor = vec4(0.0, 0.0, 0.0, 0.0);
    float vh1 = 0.0, vh2 = 0.0, vh3 = 0.0, vh4 = 0.0;


    for(int i = 0; i < 1600; i++)
    { 

        intensity =  texture(VolumeTex, voxelCoord).x; 
        voxelCoord += deltaDir;
        rayAccumulatedLength += deltaDirLen;
            
        if (rayAccumulatedLength >= rayLength ) 
        {   
            break;    
        }

        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) &&  
            ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  
            ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) )     
        {
           
            isHittingROI = true;
        
            break;
        }
    }     

    voxelCoord = EntryPoint;    
    rayAccumulatedLength = 0.0;

//  if( isHittingROI )
    {
        for(int i = 0; i < 1600; i++)
        {
            intensity =  texture(VolumeTex, voxelCoord).x;      
            int INTENSITY = int(intensity * 256);                                   

            if(  0.078125 <= intensity && 0.1171875 >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];       
                vh1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;     

            }   
            else
            if(  0.1171875 <= intensity && 0.175 >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
                vh2 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            }
            else
            if( 0.175 <= intensity && 0.234 >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
                vh3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            }
            else
            if( 0.234 <= intensity && 0.585 >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
                vh4 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            }
            
                    
            accumulatedOpacity += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            
            voxelCoord += deltaDir;
            rayAccumulatedLength += deltaDirLen;
            
            if (rayAccumulatedLength >= rayLength )
            {   
                break;  
            }   
            
            if (accumulatedOpacity >= 1.0)
            {
                break;
            }
        }                 
    }

    voxelCoord = EntryPoint;
    rayAccumulatedLength = 0.0;


    
   
    for(int i = 0; i < 1600; i++)
    {   

        intensity =  texture(VolumeTex, voxelCoord).x;                     
        int INTENSITY = int(intensity * 256);

        colorSample.r = transferFunction[INTENSITY*4 ];
        colorSample.g = transferFunction[INTENSITY*4 + 1 ];
        colorSample.b = transferFunction[INTENSITY*4 + 2 ];
        colorSample.a = transferFunction[INTENSITY*4 + 3 ];

          if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) &&  
                ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  
                ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) ) 
            {
                isHittingROI = false;
            }


        if( isHittingROI )
        {
            if(  0.078125 <= intensity && 0.1171875 >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh1),2);
            }   
            else
            if(  0.1171875 <= intensity && 0.175 >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh2),2);
            }
            else
            if( 0.175 <= intensity && 0.234 >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh3),2);    
            }
            else
            if( 0.234 <= intensity && 0.585 >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh4),2);
            }    

    
        }
        if (colorSample.a > 0.0) 
        {

            colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb * colorSample.a;
            colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;
        }
        

        voxelCoord += deltaDir;
        rayAccumulatedLength += deltaDirLen;
        
        if (rayAccumulatedLength >= rayLength )
        {   
            colorAcum.rgb = colorAcum.rgb * colorAcum.a + (1 - colorAcum.a) * bgColor.rgb;      
            break;  // terminate if opacity > 1 or; ray is outside the volume   
        }   
        else if (colorAcum.a > 1.0)
        {
            colorAcum.a = 1.0;
            break;
        }
    }
    
    FragColor = colorAcum;
}
























































/*
bool isRayHittingROI(float x, float y, float z)
{
    
    if( ( x < ROI[0] && x > ROI[3] ) && ( y < ROI[1] && y > ROI[4] ) &&  ( z < ROI[2] && z > ROI[5] ) )
        return true;
    
    return false;
}
*/

 /*
        while( indexTF < transferFunction[0] )
        {
            if( intensity <= transferFunction[indexTF] && intensity >= transferFunction[indexTF+1] )
            {
                colorSample.r = transferFunction[indexTF+2];
                colorSample.g = transferFunction[indexTF+3];
                colorSample.b = transferFunction[indexTF+4];
                colorSample.a = transferFunction[indexTF+5];
                break;
            }

         indexTF+=6;
        }    
        */


         /*
        if (intensity > 0.35 )                                             //    -------> 80/255 = 0.31 (bone)           
        {                                            
            colorSample.rgb = vec3(1.0, 0.0, 0.0);
            colorSample.a = transferFunction[0];    
        }
        else
        if (intensity > 0.07 )
        {                                                                 //     ------> 20/256 = 0.78 (tissue)
            colorSample.rgb = vec3(0.7,0.4,0.1);
            colorSample.a = transferFunction[1];
        }
        else
        {
            colorSample.rgba = vec4(0.0,0.0,0.0,0.0);
        }
    */


/*
        if( !isHittingROI )
        {
            if(  intensity <= transferFunction[indexTF] && intensity >= transferFunction[indexTF+1] )
            {
                colorSample.r = transferFunction[indexTF+2];
                colorSample.g = transferFunction[indexTF+3];
                colorSample.b = transferFunction[indexTF+4];
                colorSample.a = transferFunction[indexTF+5];    
       
            }  
            else
            if( (indexTF += 6) < transferFunction[0] && intensity <= transferFunction[indexTF] && 
                intensity >= transferFunction[indexTF+1]  )
            {
                colorSample.r = transferFunction[indexTF+2];
                colorSample.g = transferFunction[indexTF+3];
                colorSample.b = transferFunction[indexTF+4];
                colorSample.a = transferFunction[indexTF+5];
            }
            else
            if( (indexTF += 6) < transferFunction[0] && intensity <= transferFunction[indexTF] && 
                intensity >= transferFunction[indexTF+1] )
            {
                colorSample.r = transferFunction[indexTF+2];
                colorSample.g = transferFunction[indexTF+3];
                colorSample.b = transferFunction[indexTF+4];
                colorSample.a = transferFunction[indexTF+5];

            }
            else
            if( (indexTF += 6) < transferFunction[0] && intensity <= transferFunction[indexTF] && 
                intensity >= transferFunction[indexTF+1] )
            {
                colorSample.r = transferFunction[indexTF+2];
                colorSample.g = transferFunction[indexTF+3];
                colorSample.b = transferFunction[indexTF+4];
                colorSample.a = transferFunction[indexTF+5];
            }
            else
            {

                colorSample.r = 0.0;
                colorSample.g = 0.0;
                colorSample.b = 0.0;
                colorSample.a = 0.0;
            }
        }
        else
        {
            if(  intensity <= ROItransferFunction[indexTF] && intensity >= ROItransferFunction[indexTF+1] )
            {
                colorSample.r = ROItransferFunction[indexTF+2];
                colorSample.g = ROItransferFunction[indexTF+3];
                colorSample.b = ROItransferFunction[indexTF+4];
                colorSample.a = ROItransferFunction[indexTF+5];    
       
            }  
            else
            if( (indexTF += 6) < ROItransferFunction[0] && intensity <= ROItransferFunction[indexTF] && 
                intensity >= ROItransferFunction[indexTF+1]  )
            {
                colorSample.r = ROItransferFunction[indexTF+2];
                colorSample.g = ROItransferFunction[indexTF+3];
                colorSample.b = ROItransferFunction[indexTF+4];
                colorSample.a = ROItransferFunction[indexTF+5];
            }
            else
            if( (indexTF += 6) < ROItransferFunction[0] && intensity <= ROItransferFunction[indexTF] && 
                intensity >= ROItransferFunction[indexTF+1] )
            {
                colorSample.r = ROItransferFunction[indexTF+2];
                colorSample.g = ROItransferFunction[indexTF+3];
                colorSample.b = ROItransferFunction[indexTF+4];
                colorSample.a = ROItransferFunction[indexTF+5];

            }
            else
            if( (indexTF += 6) < ROItransferFunction[0] && intensity <= ROItransferFunction[indexTF] && 
                intensity >= ROItransferFunction[indexTF+1] )
            {
                colorSample.r = ROItransferFunction[indexTF+2];
                colorSample.g = ROItransferFunction[indexTF+3];
                colorSample.b = ROItransferFunction[indexTF+4];
                colorSample.a = ROItransferFunction[indexTF+5];
            }
            else
            {

                colorSample.r = 0.0;
                colorSample.g = 0.0;
                colorSample.b = 0.0;
                colorSample.a = 0.0;
            }

        }
    
*/    