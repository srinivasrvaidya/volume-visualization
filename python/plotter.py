import random
import struct
import matplotlib.pyplot as plt

i=0
value = 250
with open("stent8.raw", "r+b") as f:
    byte = f.read(1)
    while byte != "":
    	i = i+1
        # Do stuff with byte.
        byte = f.read(1)
        if byte == "":
            break;


        #data = struct.pack('B', 255)
        
        #f.write(data)        
        n = int(byte.encode('hex'), 16)	
        print n
        #if( i > 100000 and i < 110000 ):
        	#print int(byte.encode('hex'), 16)
            #    data = struct.pack('H', 255)
            #    f.write(data)
            #bin = struct.unpack('B', byte)[0]
            #print bin

        if n > 2040:
    	    #print i," ",n
            data = struct.pack('H', 241)
            f.write(data)    

f.close()
         
