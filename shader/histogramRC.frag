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

//uniform int countVoxelFlag;
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

  	float r1 = 0.0 ,r2 = 0.0 ,r3 = 0.0 ,r4 = 0.0;
    
    int count1=0, count2=0, count3=0, count4=0;

    bool rangeFlag = true;

    for(int i = 0; i < 1600; i++)
    {
    	intensity =  texture(VolumeTex, voxelCoord).x;      
        int INTENSITY = int(intensity * 256);                                   

        if(  0.078125 <= intensity && 0.1171875 >= intensity )
        {

            sampleOpacityValue = transferFunction[INTENSITY*4 + 3];       
            r1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;     

        }   
        else
        if(  0.1171875 <= intensity && 0.175 >= intensity )
        {

            sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
            r2 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        }
        else
        if( 0.175 <= intensity && 0.234 >= intensity )
        {

            sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
            r3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        }
        else
        if( 0.234 <= intensity && 0.585 >= intensity )
        {

            sampleOpacityValue = transferFunction[INTENSITY*4 + 3];
            r4 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        }
        else
        {
            //sampleOpacityValue = 0.0;
        }
      
        

        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) && 
            ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  
            ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) ) 
        {
         //   count1 += 0;    
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
    
    
//    if ( true )
	   FragColor = vec4(r1, r2, r3, r4);
//    else
//       FragColor = vec4(0.0, 0.0, 0.0, 0.0);   

//  FragColor = vec4(count1, count2, count3, count4);

    // for test
    // FragColor = vec4(EntryPoint, 1.0);
    // FragColor = vec4(exitPoint, 1.0);
   
}














































       /*
        if( intensity > 0.35 )
            r4 +=  (1.0 - accumulatedOpacity) * sampleOpacityValue;
        else
        if( intensity > 0.07 )
            r3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        else
            r1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;  
        */
/*
bool isRayHittingROI(float x, float y, float z)
{
    
    if( ( x < ROI[0] && x > ROI[3] ) && ( y < ROI[1] && y > ROI[4] ) &&  ( z < ROI[2] && z > ROI[5] ) )
        return true;
    
    return false;
}
*/

   /*
        while( indexTF < sampleOpacity[0] )
        {
            if( intensity <= sampleOpacity[indexTF] && intensity >= sampleOpacity[indexTF+1] )
            {
   
                sampleOpacityValue = sampleOpacity[indexTF+5];

            }

         indexTF+=6;  
        } 
        if( indexTF > sampleOpacity[0])
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
        
 for(int i = 0; i < 1600; i++)
    {
        intensity =  texture(VolumeTex, voxelCoord).x;                                         

        indexTF = 1;
        if(  intensity <= transferFunction[indexTF] && intensity >= transferFunction[indexTF+1] )
        {
    
            rangeFlag = true;
            break;
            
            sampleOpacityValue = transferFunction[indexTF+5];       
            r1 += (1.0 - accumulatedOpacity) * sampleOpacityValue;     

              
        }   
        else
        if( (indexTF += 6) < transferFunction[0] && intensity <= transferFunction[indexTF] && intensity >= transferFunction[indexTF+1] )
        {

    

            sampleOpacityValue = transferFunction[indexTF+5];
            r2 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        }
        else
        if( (indexTF += 6) < transferFunction[0] && intensity <= transferFunction[indexTF] && intensity >= transferFunction[indexTF+1] )
        {

            sampleOpacityValue = transferFunction[indexTF+5];
            r3 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        }
        else
        if( (indexTF += 6) < transferFunction[0] &&  intensity <= transferFunction[indexTF] && intensity >= transferFunction[indexTF+1] )
        {

            sampleOpacityValue = transferFunction[indexTF+5];
            r4 += (1.0 - accumulatedOpacity) * sampleOpacityValue;
        }
        else
        {
            sampleOpacityValue = 0.0;
        }
      
        

        if( ( voxelCoord.x < ROI[0] && voxelCoord.x > ROI[3] ) && 
            ( voxelCoord.y < ROI[1] && voxelCoord.y > ROI[4] ) &&  
            ( voxelCoord.z < ROI[2] && voxelCoord.z > ROI[5] ) ) 
        {
            count1 += 0;    
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
*/