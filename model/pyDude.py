
import random
import struct
import math

with open("foot_4.raw", "r+") as f:
#with open("skull.raw", "r+") as f:	
    #byte = f.read(1)
    data2 = struct.pack('c', chr(254))
    data1 = struct.pack('c', chr(255))
    data = struct.pack('B', 255)

    x = 0
    y = 250
    y1 = 245

    m = 150
    n = 140
    o = 150

    a=130
    b=140
    c=150

    q=140
    w=150
    e=170


    for i in range(0, 256):   # height up-down
		for j in range(0, 256):    # thickness
			for k in range(0, 256): 
				#item = i*512*174 + j*512 + k
				#f.seek(item,0)   #left-right
				byte = f.read(1)
				f.seek(-1,1)
				#f.write(struct.pack('>B', x))


				if( math.sqrt( ( (i-a)*(i-a) )  +  ( (j-b)*(j-b) ) + ( (k-c)*(k-c) )  )  <= 6):
					f.write(struct.pack('>B', y))
				elif( math.sqrt( ( (i-m)*(i-m) )  +  ( (j-n)*(j-n) ) + ( (k-o)*(k-o) )  )  <= 10):
#				if i > 200 and i < 240 and j > 80 and j < 120 and k > 200 and k < 240:	
					f.write(struct.pack('>B', y))
				elif( math.sqrt( ( (i-q)*(i-q) )  +  ( (j-w)*(j-w) ) + ( (k-e)*(k-e) )  )  <= 6):
#				if i > 200 and i < 240 and j > 80 and j < 120 and k > 200 and k < 240:	
					f.write(struct.pack('>B', y1))		
				else:
					f.write(struct.pack('>B', x))
			
f.close()
