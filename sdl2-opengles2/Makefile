CC=gcc
CPP=g++
TARGET=hello-triangle

all: $(TARGET)

$(TARGET): main.cpp
	$(CPP) -o $(TARGET) main.cpp -O2 -lm -lSDL2 -lGLESv2

clean:
	rm -rf *.o $(TARGET)