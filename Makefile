

C= g++
FLAGS =--std=c++11 -W -Wall

simulator: simulator.o instruction.o memory.o
	mkdir -p bin
	$(C) $(FLAGS) src/Simulator.o src/instruction.o src/memory.o -o bin/mips_simulator
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
	$(C) $(FLAGS) -c src/Simulator.cpp -o src/Simulator.o

instruction.o: src/instruction.cpp src/instruction.hpp
	$(C) $(FLAGS) -c src/instruction.cpp -o src/instruction.o

memory.o: src/memory.cpp src/memory.hpp
	$(C) $(FLAGS) -c src/memory.cpp -o src/memory.o

clean:
	rm -f bin/mips_simulator
	rm -f bin/test_bench
	rm -f test/temp/*.bin
	rm -f test/temp/binaries
	rm -f mips-parser/bin/parser
	




	
	
	
