
import struct
x = 255
i=0
with open('stent8.raw', 'wb') as f:
	f.seek(1000,0)

	for i in range(1000,200000):
		f.write(struct.pack('>I', x))
		print i

f.close()	