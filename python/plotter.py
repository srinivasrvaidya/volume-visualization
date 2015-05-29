import random
import struct
import matplotlib.pyplot as plt

i=0
value = 250
with open("stent.raw", "r+b") as f:
    byte = f.read(1)
    data2 = struct.pack('c', chr(254))
    data1 = struct.pack('c', chr(255))
    data = struct.pack('B', 255)
    while byte != "":
    	i = i+1
        # Do stuff with byte.
        byte = f.read(1)
        if byte == "":
			break;

        f.seek(-1,1) 
        f.write(data)   
		#data = struct.pack('B', 255)
        #f.write(data)        
        #n = int(byte.encode('hex'), 16)	
        #print n

        if int(byte.encode('hex'), 16) == 250:
            print i, " "
            #f.seek(-1,1)
            #f.write(data2)

        #if( i > 000 and i < 480000 ):
            #print i," ",
            #f.seek(-1,1)
            #print int(byte.encode('hex'), 16)
            #f.write(data)
            #f.write(struct.pack('H', (value)))
            #bin = struct.unpack('B', byte)[0]
f.close()         

'''
        if n > 2040:
    	    #print i," ",n
            data = struct.pack('H', 241)
            f.write(data)    
'''

