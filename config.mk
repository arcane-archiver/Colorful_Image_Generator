build/%.o: cs225/%.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/lodepng.o: cs225/lodepng/lodepng.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

output = main.exe
compiler = x86_64-w64-mingw32-g++
linker = ${compiler}