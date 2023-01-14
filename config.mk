build/%.o: cs225/%.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/cs225_intro.o: src/cs225_intro.cpp Makefile
	@${compiler} $(filter-out -Wconversion,${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"

build/lodepng.o: cs225/lodepng/lodepng.cpp Makefile
	@${compiler} $(filter-out ${Warnings},${Compiler_Flags}) $< -o $@ && echo -n "${Cyan}($(patsubst build/%.o,%,$@))${Nrml}"