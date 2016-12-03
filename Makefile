
CC=mingw32-c++.exe
	
clulphate:
	$(CC) -std=c++14 -I C:/libs/glfw-3.2.1/include/ clulphate.cpp -L C:/libs/glfw-3.2.1/lib -lglfw3 -lgdi32 -lopengl32 -lm -g -o clulphate
