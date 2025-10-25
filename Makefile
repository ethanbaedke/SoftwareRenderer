EXECUTABLE = app
BUILD_FOLDER = build
SRC = src/main.c src/constants.c src/logger.c src/console_display.c src/x11_display.c src/rasterizer.c

${BUILD_FOLDER}/${EXECUTABLE}: ${SRC}
	gcc -Iinclude -g ${SRC} -o ${BUILD_FOLDER}/${EXECUTABLE} -lX11 -lm

clean:
	rm -f ${BUILD_FOLDER}/${EXECUTABLE}