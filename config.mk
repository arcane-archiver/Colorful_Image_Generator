build/%.o: cs225/%.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/lodepng.o: cs225/lodepng/lodepng.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"