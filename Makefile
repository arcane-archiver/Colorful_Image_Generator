.DEFAULT_GOAL := run/colorful
.PHONY: clean

-include config.mk

COMPILER := clang++
COMPILER_FLAG_LIST := -std=c++17 $(shell sdl2-config --cflags) ${WARNING_FLAG_LIST}
LINKER_FLAG_LIST := $(shell sdl2-config --libs)

SOURCE_LIST := $(wildcard source/*.cpp)
OBJECT_LIST := $(patsubst source/%.cpp,build/%.o,${SOURCE_LIST})

run/colorful: ${OBJECT_LIST}
	${COMPILER} $^ -o $@ ${LINKER_FLAG_LIST}

${OBJECT_LIST}: build/%.o: source/%.cpp
	${COMPILER} ${COMPILER_FLAG_LIST} -c $< -o $@

clean:
	rm --force run/*
	rm --force build/*
