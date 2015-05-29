	
accumulatedOpacity = 0.0
sampleOpacityValue = 0.9
i = 0
r1 = 0.0

while ( i < 90 ):

	i=i+1

	

	if ( accumulatedOpacity >= 1.0):
		break;

	print i, " addition  ",(1.0 - accumulatedOpacity) * sampleOpacityValue, "  "	

	r1 = r1 + ( (1.0 - accumulatedOpacity) * sampleOpacityValue )


	print r1

	accumulatedOpacity += (1.0 - accumulatedOpacity) * sampleOpacityValue	 
