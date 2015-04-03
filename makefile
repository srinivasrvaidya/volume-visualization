CC = g++
OFLAG = -o
CFLAGS = -c -Wall -g -DDEBUG 
LIB = -lGLEW -lGL -lGLU -lglut -lc -lm -lstdc++ -Wno-write-strings
LDFLAGS = `pkg-config --libs opencv`

all: main.o
	$(CC) $(OFLAG) main main.o  $(LIB_DIR) $(LIB) $(WIN_GUI_FLAG) $(LDFLAGS) 
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(INC_DIR)
clean:
	rm main.o main