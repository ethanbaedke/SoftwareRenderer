EXECUTABLE = app
BUILD_FOLDER = build
SRC = src/main.c src/constants.c src/console_display.c src/rasterizer.c

${BUILD_FOLDER}/${EXECUTABLE}: ${SRC}
	gcc -Iinclude ${SRC} -o ${BUILD_FOLDER}/${EXECUTABLE} -lm

clean:
	rm -f ${BUILD_FOLDER}/${EXECUTABLE}