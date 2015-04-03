#version 130


in vec3 EntryPoint;
in vec4 ExitPointCoord;

uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;  
uniform float     StepSize;
uniform vec2      ScreenSize;
uniform float sampleOpacity[50];
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
    vec4 colorAcum = vec4(0.0); // The dest color
    float alphaAcum = 0.0;                // The  dest alpha for blending
    float intensity;
    float rayAccumulatedLength = 0.0;
    vec4 colorSample; // The src color 
    float alphaSample; // The src alpha
    int j;  
    bool flagOther = true;


    vec4 bgColor = vec4(0.0, 0.0, 0.0, 0.0);
     
        for(int i = 0; i < 2000; i++)
        {   

        	intensity =  texture(VolumeTex, voxelCoord).x;                     
            j = 1;
            flagOther = true;

            if(  intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
            {
                colorSample.r = sampleOpacity[j+2];
                colorSample.g = sampleOpacity[j+3];
                colorSample.b = sampleOpacity[j+4];
                colorSample.a = sampleOpacity[j+5];
                flagOther = false;
                
            }   

            j += 6;
            if(  intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
            {
                colorSample.r = sampleOpacity[j+2];
                colorSample.g = sampleOpacity[j+3];
                colorSample.b = sampleOpacity[j+4];
                colorSample.a = sampleOpacity[j+5];
                flagOther = false;
            }

            j += 6;
            if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
            {
                colorSample.r = sampleOpacity[j+2];
                colorSample.g = sampleOpacity[j+3];
                colorSample.b = sampleOpacity[j+4];
                colorSample.a = sampleOpacity[j+5];
                flagOther = false;
            }
            
            j += 6;
            if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
            {
                colorSample.r = sampleOpacity[j+2];
                colorSample.g = sampleOpacity[j+3];
                colorSample.b = sampleOpacity[j+4];
                colorSample.a = sampleOpacity[j+5];
                flagOther = false;
            }
            if ( flagOther )
            {

                colorSample.r = 0.0;
                colorSample.g = 0.0;
                colorSample.b = 0.0;
                colorSample.a = 0.0;
            }

    	if (colorSample.a > 0.0) 
    	{

    	    colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb * colorSample.a;
    	    colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;
		}
		
        
        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) && ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) )
        {

       //     colorAcum.rgba = vec4(colorAcum.r/2.0, colorAcum.g/2.0, colorAcum.b/2.0, colorAcum.a/2.0 );
            //      colorAcum.rgba = vec4(0.8, 0.2, 0.1, 1.0 );
         //     break;   
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
 
    // for test
    // FragColor = vec4(EntryPoint, 1.0);
    // FragColor = vec4(exitPoint, 1.0);
   
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
        while( j < sampleOpacity[0] )
        {
            if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
            {
                colorSample.r = sampleOpacity[j+2];
                colorSample.g = sampleOpacity[j+3];
                colorSample.b = sampleOpacity[j+4];
                colorSample.a = sampleOpacity[j+5];
                break;
            }

         j+=6;
        }    
        */


         /*
        if (intensity > 0.35 )                                             //    -------> 80/255 = 0.31 (bone)           
        {                                            
            colorSample.rgb = vec3(1.0, 0.0, 0.0);
            colorSample.a = sampleOpacity[0];    
        }
        else
        if (intensity > 0.07 )
        {                                                                 //     ------> 20/256 = 0.78 (tissue)
            colorSample.rgb = vec3(0.7,0.4,0.1);
            colorSample.a = sampleOpacity[1];
        }
        else
        {
            colorSample.rgba = vec4(0.0,0.0,0.0,0.0);
        }
    */