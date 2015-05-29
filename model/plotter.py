import random
import struct
import matplotlib.pyplot as plt

i=0
value = 250
arrList = []
'''
for i in range(60, 90):
    for j in range(200, 250):
        for k in range(200, 250):
            item = i*174 + j*512 + k
            arrList.append(item)
'''
#print arrList

#fp = open("stent.raw", "wb")

#with open("stent8.raw", "r+") as f:
with open("angio.raw", "r+") as f:    

    i=0
    j=0
    k=0
    counter = 0
    byte = f.read(1)
    data2 = struct.pack('c', chr(254))
    data1 = struct.pack('c', chr(255))
    data = struct.pack('B', 255)

    while byte != "":

        # Do stuff with byte.
        byte = f.read(1)
        if byte == "":
            break;
    	
        counter = counter+1    
        k = k + 1      

        if counter % (512*174) == 0:
            i = i + 1
            j = 0
            k = 0

        if counter % 174 == 0:
            j = j + 1
            k = 0


        y = 0    
        if i == -80 and i < 120 and j > 80 and j < 120 and k > 80 and k < 120:
            #print i, j, k
            f.seek(-1,1)    
            f.write(struct.pack('>B', y))    

        #f.seek(-1,1) 
        #f.write(data)  
		#data = struct.pack('B', 255)
        #f.write(data)        
        
        x = int(byte.encode('hex'), 16)	
        if x != 0:
            print x," - ", i, j, k

        #fp.write(struct.pack('>I', x))
        #fp.write(struct.pack('>B', x))
        #fp.write(bin(255)[2:])

        #if int(byte.encode('hex'), 16) == 250:
            #print i, " ",


        if int(byte.encode('hex'), 16) >= 240:
            #print i, " ",
            f.seek(-1,1)
            #f.write(data2)
            x = 235
            f.write(struct.pack('>B', x))


        #if ( i > 44440000 and i < 44541705 ):
        #if i in arrList:
        if i == -10: 
            #print i," ",
            #f.seek(-1,1)
            print int(byte.encode('hex'), 16)
            #f.write(data)
            #x = 255
            #f.write(struct.pack('>B', x))
            #f.write(struct.pack('>I', x))
            #x=255
            #f.write(bytes([x]))
            #f.write(struct.pack('H', (value)))
            #bin = struct.unpack('B', byte)[0]
f.close()
#fp.close()         
print "out"
'''
        if n > 2040:
    	    #print i," ",n
            data = struct.pack('H', 241)
            f.write(data)    
'''

