MIPS Processor Simulator
----

**1. THE PROJECT**

MIPS processor simulation written in C++, along with testbench written in bash script to ascertain functionality of similar softwares.  


**2. FUNCTIONALITY**

Mimics a MIPS processor in software by reading from an input binary file and performing the associated instructions. Modifies and reads from registers, memory and generates exceptions just like a MIPS 1 processor. Generates a return code based on actions performed.

**3. REQUIRED SOFTWARE**

* C++ compiler (like gcc)
* Preferably use a Linux OS. 

**4. HOW TO RUN**

Download and use command in the terminal window:
```
make testbench
```
to create a testbench executable called `bin/mips_testbench`. Use command:
```
make simulator
```
to create an executable of the simulator called `bin/mips_simulator`.
Then you can run the simulator on a binary file using command:
```
bin/mips_simulator xxx.bin
```
or test it with the testbench:
```
bin/mips_testbench bin/mips_simulator
```
Similar testbenches/ simulators may also be used.


**MORE INFORMATION**

This project is based on [this spec](https://github.com/m8pple/arch2-2019-cw). It was done as part of an assessment, but the implementation of the spec is the original work of the authors.

**ACKNOWLEDGEMENTS**

We are grateful to **Olly Larkin** whose parser [(accessible here)](https://github.com/olly-larkin/mips-parser) we used to translate mips assembly in *.txt* files to binaries for testing.
