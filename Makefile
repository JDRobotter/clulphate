
CC=mingw32-c++.exe
CFLAGS=-std=c++14 -Wall -O3
INCLUDES=-I "C:/libs/glfw-3.2.1/include/" -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS=-L "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\lib\Win32" -L "C:/libs/glfw-3.2.1/lib"

clulphate: clulphate.o
	$(CC) $(CFLAGS) $(LIBS) $^ -lOpenCL -lglfw3 -lgdi32 -lopengl32 -lm -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@

ocltest: ocltest.o
	$(CC) $(CFLAGS) $(LIBS) $^ -lOpenCL -o $@

ocltest2: ocltest2.o
	$(CC) $(CFLAGS) $(LIBS) $^ -lOpenCL -o $@
