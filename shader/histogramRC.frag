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
  
    float intensity;
    float rayAccumulatedLength = 0.0;
    float accumulatedOpacity=0.0;
    float sampleOpacityValue = 0.0;
    bool flagOther = true;

  	float r1 = 0.0 ,r2 = 0.0 ,r3 = 0.0 ,r4 = 0.0;
    int j;

    for(int i = 0; i < 2600; i++)
    {
    	intensity =  texture(VolumeTex, voxelCoord).x;   // --->

        j = 1;
        
        if(  intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            sampleOpacityValue = sampleOpacity[j+5];
            flagOther = false;
                
        }   

        j += 6;
        if(  intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            sampleOpacityValue = sampleOpacity[j+5];
            flagOther = false;
        }

        j += 6;
        if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            sampleOpacityValue = sampleOpacity[j+5];
            flagOther = false;
        }
            
        j += 6;
        if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            sampleOpacityValue = sampleOpacity[j+5];
            flagOther = false;
        }

        if ( flagOther )
        {
            sampleOpacityValue = 0.0;
        }
      
        

        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) && ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) ) 
        {
            
        }

        accumulatedOpacity += (1.0 - accumulatedOpacity) * sampleOpacityValue;
		
        /*
		if( intensity > 0.35 )
			r4 +=  (1.0 - accumulatedOpacity) * sampleOpacityValue;
		else
		if( intensity > 0.07 )
			r3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
		else
			r1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;	
		*/


        flagOther  = true;
        j=1;

        if(  intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            r1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;  
            flagOther = false;
        }   

        j += 6;
        if(  intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            r2 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            flagOther = false;
        }

        j += 6;
        if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            r3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            flagOther = false;
        }
            
        j += 6;
        if( intensity <= sampleOpacity[j] && intensity >= sampleOpacity[j+1] )
        {
            r4 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
            flagOther = false;
        }

	
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
    
    
	FragColor = vec4(r1, r2, r3, r4);

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
   
                sampleOpacityValue = sampleOpacity[j+5];

            }

         j+=6;  
        } 
        if( j > sampleOpacity[0])
        {
            sampleOpacityValue = 0.0;
        }

*/

/*                    
        if (intensity > 0.35 )                            //    -------> 80/255 = 0.31 (bone)
        {                         
           sampleOpacityValue = sampleOpacity[0];
        }
        else
        if (intensity > 0.07 )
        {                                                //    ------> 20/256 = 0.78 (tissue)
           sampleOpacityValue = sampleOpacity[1];
        }
        else
        {
           sampleOpacityValue = 0.0;    
        }
*/        