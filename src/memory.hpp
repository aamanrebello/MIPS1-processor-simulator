#include<map>
#include<vector>
#include "stdlib.h"

// Defines instruction, data and output memory as well as instructions that operate on this memory.

#define ADDR_NULL 0x000000000
#define MEM_LIMIT 0x100000000
#define ADDR_INSTR 0x10000000
#define INSTR_SIZE 0x1000000
#define ADDR_DATA 0x20000000
#define DATA_SIZE 0x4000000
#define ADDR_GETC 0x30000000
#define ADDR_PUTC 0x30000004

class memory
{
private:
  //Instruction memory is a vector
  std::vector<char> instructions;
  // Difficult to manage data memory in the same way as it is very large so instead we use a map to
  // record which addresses have beenr written to -> more efficient.
  std::map<int, char> stored_data;
  // Output/input memory.
  char GETCmemory[4] = {0, 0, 0, 0};
  char PUTCmemory[4] = {0, 0, 0, 0};
  // The PC is the only relevant variable here. NPC is a kind of support variable that we use to help
  // ensure correct behaviour of the PC under all conditions - kind of a PC lookahead.
  int PC = ADDR_INSTR;
  int NPC = ADDR_INSTR + 4;
  // Memory size of loaded instructions.
  int current_instr_size = 0;

public:
  // Reads the binary instructions and stores into instruction memory.
  bool read_and_store_instructions(std::string filename);
  // Simple getter method for current_instr_size.
  int get_instr_size();
  // Loads instruction based on position of PC.
  std::int32_t load_instruction();
  // Reads byte from instruction memory.
  std::int32_t read_instr_byte(int address);
  // Reads byte/ half word/ word from memory - format = 'u' for unsigned, 's' for signed.
  std::int32_t load_byte(int address, char format) const;
  std::int32_t load_half_word(int address, char format) const;
  std::int32_t load_word(int address) const;//C
  std::int32_t load_word_left(int address, int register_value) const;//C
  std::int32_t load_word_right(int address, int register_value) const;//C
  // Stores byte/ half word/ word from memory.
  int store_byte(int address, std::int32_t word);
  int store_half_word(int address, std::int32_t word);
  int store_word(int address, std::int32_t word);
  // Operates on input and output memory
  std::int32_t GETCreadbyte();
  int PUTCwritebyte(std::int32_t word);
  // Related with PC manipulation.
  int advancePC(int newNPC);
  int getPC();
  int getNPC();
};
