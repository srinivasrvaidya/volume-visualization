#version 130


in vec3 EntryPoint;
in vec4 ExitPointCoord;

uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler3D Volume2Tex;
uniform sampler1D TransferFunc;  
uniform float     StepSize;
uniform vec2      ScreenSize;
uniform float transferFunction[256*4];
uniform float ROItransferFunction[50];
uniform float ROI[6];
uniform int r[8];

//layout (location = 0) 
out vec4 FragColor;

#define WIN_SIZE 10

int gradientOfWindow(int w[WIN_SIZE])
{
    
    int min=0, max=0;

    for(int i=0; i < WIN_SIZE; i++)
    {
        if(min > w[i])
            min = w[i];


        if(max < w[i])
            max = w[i];
    }


    return (max-min);
}


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
    float intensity, intensity2;
    float rayAccumulatedLength = 0.0;
    vec4 colorSample; // The src color 
    float alphaSample; // The src alpha
    bool flag = false;
    bool isHittingROI = false;
    bool isoSurfaceFlag = false;
   
    float sampleOpacityValue = 0.0;
    float accumulatedOpacity = 0.0;

    vec4 bgColor = vec4(0.0, 0.0, 0.0, 0.0);
    float vh1 = 0.0, vh2 = 0.0, vh3 = 0.0, vh4 = 0.0;
    float range[8];

/*
    range[0] = 60;    range[1] = 80;
    range[2] = 80;    range[3] = 100;
    range[4] = 100;   range[5] = 120;
    range[6] = 150;   range[7] = 256;
 

    range[0] = 20;    range[1] = 30;
    range[2] = 30;    range[3] = 45;
    range[4] = 45;    range[5] = 60;
    range[6] = 60;    range[7] = 256;
*/



    range[0] = r[0] / 256;
    range[1] = r[1] / 256;
    
    range[2] = r[2] / 256;
    range[3] = r[3] / 256;

    range[4] =  r[4] / 256;
    range[5] =  r[5] / 256;

    range[6] =  r[6] / 256;
    range[7] =  r[7] / 256;



    for(int i = 0; i < 2256; i++)
    { 

        intensity =  texture(VolumeTex, voxelCoord).x;
        intensity2 =  texture(Volume2Tex, voxelCoord).x; 

        voxelCoord += deltaDir;
        rayAccumulatedLength += deltaDirLen;
            
        if (rayAccumulatedLength >= rayLength ) 
        {   
            break;    
        }

        // bounding box.
        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) &&  
            ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  
            ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) )                
        {
    //        isHittingROI = true;
            break;
        }

        if ( intensity > 0.234 )
        {
   //         isHittingROI = true;
   //         break;
        }
       
        if( intensity2 > 0.5 )     
        {
            isHittingROI = true;
            break;
        }
        
    }     

    voxelCoord = EntryPoint;    
    rayAccumulatedLength = 0.0;


        for(int i = 0; i < 2600; i++)  // computing visibility histogram. for per-ray VH on ROI.
        {
            intensity =  texture(VolumeTex, voxelCoord).x;      
            int INTENSITY = int(intensity * 256);                                   

            if(  range[0] <= intensity && range[1] >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];       
                vh1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;     

            }   
            else
            if(  range[2] <= intensity && range[3] >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
                vh2 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            }
            else
            if( range[4] <= intensity && range[5] >= intensity )
            {

                sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
                vh3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            }
            else
            if( range[6] <= intensity && range[7] >= intensity )
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
    


    voxelCoord = EntryPoint;
    rayAccumulatedLength = 0.0;
    accumulatedOpacity = 0;
    

    int winSize = WIN_SIZE, ptr=0;
    int window[WIN_SIZE];
    if ( isoSurfaceFlag)
    {
        for(int i = 0; i < winSize; i++)
        // Initialization of window for iso-surface extraction using ray casting.
        {   
            intensity =  texture(VolumeTex, voxelCoord).x;                     
            int INTENSITY = int(intensity * 256);
            voxelCoord += deltaDir;
            rayAccumulatedLength += deltaDirLen;
            window[i] = INTENSITY;
        } 
    }


    int n;
    for(int i = 0; i < 2600; i++)
    {   

        intensity =  texture(VolumeTex, voxelCoord).x;  
        intensity2 = texture(Volume2Tex, voxelCoord).x;                    
        int volSampleIntensity = int( texture(VolumeTex, voxelCoord).x * 256);    
        int vol2SampleIntensity = int( texture(Volume2Tex, voxelCoord).x * 256);
        
        /*
        if( gradientOfWindow(window) < 0 && isoSurfaceFlag)
        {
            
            window[ptr%winSize] = volSampleIntensity;
            ptr++;

            voxelCoord += deltaDir;
            rayAccumulatedLength += deltaDirLen;
            continue;
        }
        */

        colorSample.r = transferFunction[volSampleIntensity*4 ];
        colorSample.g = transferFunction[volSampleIntensity*4 + 1 ];
        colorSample.b = transferFunction[volSampleIntensity*4 + 2 ];
        colorSample.a = transferFunction[volSampleIntensity*4 + 3 ];

        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) &&  
            ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  
            ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) ) 
        {
   //          isHittingROI = false;

            // Ray hit the ROI and normal composting begins.
        }


        if( vol2SampleIntensity >= 240)
        {
        
            isHittingROI = false;

            colorSample.r = 0.0;
            colorSample.g = 1.0;
            colorSample.b = 0.0;
            colorSample.a = 1.0;

            colorSample.a = colorSample.a * pow(( 1 - vh4),2);

        }


        n = 4;
        if( isHittingROI )
        {

            if(  range[0] <= intensity && range[1] >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh1), n);
            }   
            else
            if(  range[2] <= intensity && range[3] >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh2), n);
            }
            else
            if(  range[4] <= intensity && range[5] >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh3), n);    
            }
            else
            if(  range[6] <= intensity && range[7] >= intensity )
            {
                colorSample.a = colorSample.a * pow(( 1 - vh4), n);
            }        

        }
      
        
        if (colorSample.a > 0.0) 
        {
        // Front to back composting.
            colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb * colorSample.a;
            colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;
        }

        

/*      if( isoSurfaceFlag)
        {
            for(int j = 0; j < winSize; j++)
            {   
                intensity =  texture(VolumeTex, voxelCoord).x;                     
                int INTENSITY = int(intensity * 256);
                voxelCoord += deltaDir;
                rayAccumulatedLength += deltaDirLen;
                window[j] = INTENSITY;
                i = i+5;
                ptr = 0;
            }
        }
*/
        

        voxelCoord += deltaDir;
        rayAccumulatedLength += deltaDirLen;
        
        if (rayAccumulatedLength >= rayLength )
        {   
            colorAcum.rgb = colorAcum.rgb * colorAcum.a + (1 - colorAcum.a) * bgColor.rgb;      
            break;      // terminate if opacity > 1 or; ray is outside the volume   
        }   
        else if (colorAcum.a > 1.0)
        {
            colorAcum.a = 1.0;
            break;
        }
    }

    
    
    FragColor = colorAcum;
}
