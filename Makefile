all:
	clang++ -g -std=c++11 -stdlib=libstdc++ -L../LeapSDK/lib/x64 -lLeap -lGL -lglut -lfreeimage -lGLEW -o leapimage opengl.cpp main.cpp 
