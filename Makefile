BUILD_DIR := build/local
NAME := colorful
.DEFAULT_GOAL := ${BUILD_DIR}/${NAME}
.PHONY: all clean

-include config.mk

COMPILER := clang++
COMPILER_FLAG_LIST := -std=c++17 $(shell sdl2-config --cflags) ${WARNING_FLAG_LIST}
LINKER_FLAG_LIST := $(shell sdl2-config --libs)

SOURCE_LIST := $(wildcard source/*.cpp)
OBJECT_LIST := $(patsubst source/%.cpp,${BUILD_DIR}/%.o,${SOURCE_LIST})
DEP_LIST := $(OBJECT_LIST:.o=.d)

all: ${.DEFAULT_GOAL} build/web/${NAME}.wasm
	echo ${DEP_LIST}

build/web/${NAME}.wasm:
	make \
		BUILD_DIR:=build/web \
		NAME:=colorful \
		COMPILER:=em++ \
		COMPILER_FLAG_LIST:='-std=c++17 -sUSE_SDL=2 ${WARNING_FLAG_LIST}' \
		LINKER_FLAG_LIST:='-sUSE_SDL=2'

${.DEFAULT_GOAL}: ${OBJECT_LIST}
	${COMPILER} $^ -o $@ ${LINKER_FLAG_LIST}

${OBJECT_LIST}: ${BUILD_DIR}/%.o: source/%.cpp ${BUILD_DIR}
	${COMPILER} ${COMPILER_FLAG_LIST} -c $< -o $@

${BUILD_DIR}:
	mkdir --parents $@

clean:
	rm --force --recursive build/*
