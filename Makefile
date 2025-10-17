EXECUTABLE = app
BUILD_FOLDER = build

${BUILD_FOLDER}/${EXECUTABLE}: src/main.c
	gcc src/main.c -o ${BUILD_FOLDER}/${EXECUTABLE}

clean:
	rm -f ${BUILD_FOLDER}/${EXECUTABLE}