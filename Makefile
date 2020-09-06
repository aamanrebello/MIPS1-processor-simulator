

C= g++
FLAGS =--std=c++11 -W -Wall

simulator: simulator.o instruction.o memory.o
	mkdir -p bin
	$(C) $(FLAGS) bin/Simulator.o bin/instruction.o bin/memory.o -o bin/mips_simulator
	chmod u+x bin/mips_simulator

parser:
	cd ./mips-parser && make
	cd ..

testbench: parser
	mkdir -p bin
	mkdir -p test/temp
	cp tb/mips_testbench bin/
	chmod u+x bin/mips_testbench

	

simulator.o: src/Simulator.cpp 
	$(C) $(FLAGS) -c src/Simulator.cpp -o bin/Simulator.o

instruction.o: src/instruction.cpp src/instruction.hpp
	$(C) $(FLAGS) -c src/instruction.cpp -o bin/instruction.o

memory.o: src/memory.cpp src/memory.hpp
	$(C) $(FLAGS) -c src/memory.cpp -o bin/memory.o

clean:
	rm -f bin/mips_simulator
	rm -f bin/mips_testbench
	rm -f bin/*.o
	rm -f test/temp/*.bin
	rm -f test/temp/binaries
	rm -f mips-parser/bin/parser
	




	
	
	
