basename = generate_colorful_image
programName = ${basename}.out

compiler = clang++
linker = ${compiler}

outputDir = build

# Include custom configuration if it exists.
-include config.mk

.DEFAULT_GOAL = ${build}/${programName}

.PHONY: clean

DepfileFlags = -MMD -MP

# Warnings = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wconversion

CompilerFlags = -std=c++20 -O0 ${Warnings} ${DepfileFlags} -g -c -I hdr/

StaticLibrarues = $(patsubst lib/lib%.a,-l%,$(wildcard lib/*.a))

Linker_Flags = -std=c++17 -Llibrary/

Objects = $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp)) $(patsubst cs225/%.cpp,build/%.o,$(wildcard cs225/*.cpp)) build/lodepng.o

Objects_for_Testing = $(filter-out build/main.o,${Objects}) build/catchmain.o $(patsubst tests/%.cpp,build/%.o,$(wildcard tests/*.cpp))

Dependencies := $(patsubst build/%.o,build/%.d,${Objects})

# these are for making programName look pretty
Ylow = \e[0;33m
Prpl = \e[0;35m
Blue = \e[0;34m
Gren = \e[0;32m
Cyan = \e[0;36m
Red_ = \e[0;31m # The underscore after red is so that all the colors have four characters. ^_~
Nrml = \e[0m
BlOpBrac = ${Blue}{${Cyan}
BlClBrac = ${Blue}}${Nrml}

all: bin/${programName} bin/TEST_${programName}
	@echo "${Gren}Everything is compiled.${Nrml}"

main: bin/${programName}
	@echo "${Prpl}[$@]${Nrml}"
	@${runner} ./${outputDir}/$build{programName} ${args}

test: bin/TEST_${programName}
	@echo "${Prpl}[$@]${Nrml}"
	@${runner} ./bin/TEST_${programName} ${args}

${outputDir}/$build{programName}: ${Objects}
	@${linker} ${Linker_Flags} $^ ${StaticLibrarues} ${libs} -o $@ \
	&& echo "${Gren}[main]${Nrml}"

bin/TEST_${programName}: ${Objects_for_Testing}
	@${linker} ${Linker_Flags} $^ ${StaticLibrarues} ${libs} -o $@ \
	&& echo "${Gren}[test]${Nrml}"

-include $(Dependencies)

build/%.o: src/%.cpp Makefile
	@${compiler} ${CompilerFlags} $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/%.o: tests/%.cpp Makefile
	@${compiler} ${CompilerFlags} $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/catchmain.o: tests/catch/catchmain.cpp Makefile
	@${compiler} ${CompilerFlags} $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

clean:
	@echo "${Blue}clean bin ${BlOpBrac}$(patsubst bin/%,%,$(filter-out .gitkeep,$(wildcard bin/*)))${BlClBrac}"
	@rm --force $(filter-out .gitkeep,$(wildcard bin/*))
	@echo "${Blue}clean build ${BlOpBrac}$(patsubst build/%,%,$(filter-out .gitkeep,$(wildcard build/*)))${BlClBrac}"
	@rm --force $(filter-out .gitkeep,$(wildcard build/*))

list:
	@echo "programName: ${programName}"
	@echo "target directory: ${outputDir}"
build	@echo "compiler: ${compiler}"
	@echo "linker: ${linker}"
	@echo "runner: ${runner}"
	@echo "${Blue}headers ${BlOpBrac}$(patsubst hdr/%.hpp,%,$(wildcard hdr/*.hpp))${BlClBrac}"
	@echo "${Blue}sources ${BlOpBrac}$(patsubst src/%.cpp,%,$(wildcard src/*.cpp))${BlClBrac}"
	@echo "${Blue}template definitions ${BlOpBrac}$(patsubst src/%.tpp,%,$(wildcard src/*.tpp))${BlClBrac}"
	@echo "${Blue}static libraries ${BlOpBrac}$(patsubst -l%,%,${StaticLibrarues})${BlClBrac}"
	@echo "${Blue}dynamic libraries ${BlOpBrac}$(patsubst %,%,${libs})${BlClBrac}"
	@echo "${Blue}target objects ${BlOpBrac}$(patsubst build/%.o,%,${Objects})${BlClBrac}"
	@echo "${Blue}test objects ${BlOpBrac}$(patsubst build/%.o,%,${Objects_for_Testing})${BlClBrac}"
