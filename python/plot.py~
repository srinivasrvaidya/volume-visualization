import random
import struct
import matplotlib.pyplot as plt

i=0;
with open("stent8.raw", "r+") as f:
    byte = f.read(1)
    while byte != "":
		i = i+1
		byte = f.read(1) #print int(byte.encode('hex'), 16)
		if( i > 18000 and i < 20000 ):
			f.write(struct.pack('i', 250)

