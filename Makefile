TARGET=leapimage
CC=clang++
CFLAGS=-g -std=c++11 -stdlib=libstdc++
LIBS=-L../LeapSDK/lib/x64 -lLeap -lGL -lglut -lfreeimage -lGLEW
FILES=src/*.cpp

${TARGET}:
	${CC} ${CFLAGS} ${LIBS} -o ${TARGET} ${FILES}
	mv ${TARGET} build/

all: ${TARGET}

clean:
	rm ${TARGET}
