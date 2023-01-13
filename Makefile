name = main
output = ${name}.out
out_dir = bin
compiler = clang++
linker = ${compiler}
-include config.mk

.DEFAULT_GOAL = bin/${output}
.PHONY: all archive main test clean list

Depfile_Flags = -MMD -MP
Warnings = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wconversion
Compiler_Flags = -std=c++20 -O0 ${Warnings} ${Depfile_Flags} -g -c -I hdr/
Static_Libs = $(patsubst lib/lib%.a,-l%,$(wildcard lib/*.a))
Linker_Flags = -std=c++20 -Llib/

Objects = $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp)) $(patsubst cs225/%.cpp,build/%.o,$(wildcard cs225/*.cpp)) build/lodepng.o
Objects_for_Testing = $(filter-out build/main.o,${Objects}) build/catchmain.o $(patsubst tests/%.cpp,build/%.o,$(wildcard tests/*.cpp))

Dependencies := $(patsubst build/%.o,build/%.d,${Objects})

# these are for making output look pretty
Ylow = \e[0;33m
Prpl = \e[0;35m
Blue = \e[0;34m
Gren = \e[0;32m
Cyan = \e[0;36m
Red_ = \e[0;31m # The underscore after red is so that all the colors have four characters. ^_~
Nrml = \e[0m
BlOpBrac = ${Blue}{${Cyan}
BlClBrac = ${Blue}}${Nrml}

all: bin/${output} bin/TEST_${output}
	@echo "${Gren}Everything is compiled.${Nrml}"

main: bin/${output}
	@echo "${Prpl}[$@]${Nrml}"
	@${runner} ./${out_dir}/${output} ${args}

test: bin/TEST_${output}
	@echo "${Prpl}[$@]${Nrml}"
	@${runner} ./bin/TEST_${output} ${args}

${out_dir}/${output}: ${Objects}
	@${linker} ${Linker_Flags} $^ ${Static_Libs} ${libs} -o $@ \
	&& echo "${Gren}[main]${Nrml}"

bin/TEST_${output}: ${Objects_for_Testing}
	@${linker} ${Linker_Flags} $^ ${Static_Libs} ${libs} -o $@ \
	&& echo "${Gren}[test]${Nrml}"

-include $(Dependencies)

build/%.o: src/%.cpp Makefile
	@${compiler} ${Compiler_Flags} $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/%.o: tests/%.cpp Makefile
	@${compiler} ${Compiler_Flags} $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/%.o: cs225/%.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/catchmain.o: tests/catch/catchmain.cpp Makefile
	@${compiler} ${Compiler_Flags} $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/lodepng.o: cs225/lodepng/lodepng.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

clean:
	@echo "${Blue}clean bin ${BlOpBrac}$(patsubst bin/%,%,$(filter-out .gitkeep,$(wildcard bin/*)))${BlClBrac}"
	@rm --force $(filter-out .gitkeep,$(wildcard bin/*))
	@echo "${Blue}clean build ${BlOpBrac}$(patsubst build/%,%,$(filter-out .gitkeep,$(wildcard build/*)))${BlClBrac}"
	@rm --force $(filter-out .gitkeep,$(wildcard build/*))

list:
	@echo "output: ${output}"
	@echo "target directory: ${out_dir}"
	@echo "compiler: ${compiler}"
	@echo "linker: ${linker}"
	@echo "runner: ${runner}"
	@echo "${Blue}headers ${BlOpBrac}$(patsubst hdr/%.hpp,%,$(wildcard hdr/*.hpp))${BlClBrac}"
	@echo "${Blue}sources ${BlOpBrac}$(patsubst src/%.cpp,%,$(wildcard src/*.cpp))${BlClBrac}"
	@echo "${Blue}template definitions ${BlOpBrac}$(patsubst src/%.tpp,%,$(wildcard src/*.tpp))${BlClBrac}"
	@echo "${Blue}static libraries ${BlOpBrac}$(patsubst -l%,%,${Static_Libs})${BlClBrac}"
	@echo "${Blue}dynamic libraries ${BlOpBrac}$(patsubst %,%,${libs})${BlClBrac}"
	@echo "${Blue}target objects ${BlOpBrac}$(patsubst build/%.o,%,${Objects})${BlClBrac}"
	@echo "${Blue}test objects ${BlOpBrac}$(patsubst build/%.o,%,${Objects_for_Testing})${BlClBrac}"
