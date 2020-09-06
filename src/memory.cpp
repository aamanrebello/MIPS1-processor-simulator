#include<fstream>
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include "memory.hpp"


bool memory::read_and_store_instructions(std::string filename)//TESTED
{
    std::ifstream myfile;
    myfile.open(filename, std::ios::binary|std::ios::ate);

    if(myfile.is_open())
    {
        int size = myfile.tellg();

        instructions.resize(size);
        current_instr_size = size;
        myfile.seekg (0, std::ios::beg);
        myfile.read (&instructions[0], size);
        myfile.close();
        return true;
    }
    else
    {
        return false;
    }
}

int memory::get_instr_size()//TESTED
{
    return current_instr_size;
}


std::int32_t memory::load_instruction()//TESTED
{
    int arraypos = PC - ADDR_INSTR;
    if((PC >= ADDR_INSTR)&&(PC < ADDR_INSTR + INSTR_SIZE))
    {
      if(PC < ADDR_INSTR + current_instr_size - 1)
      {
        int byte0 = ((unsigned int)instructions[arraypos + 3])&(0x000000FF);
        int byte1 = (((unsigned int)instructions[arraypos + 2])<<8)&(0x0000FF00);
        int byte2 = (((unsigned int)instructions[arraypos + 1])<<16)&(0x00FF0000);
        int byte3 = (((unsigned int)instructions[arraypos])<<24)&(0xFF000000);
        std::int32_t instruction = byte0|byte1|byte2|byte3;
        //PC = PC+4 in main();
        return instruction;
      }
      else
      {
        return 0;
      }
    }
    else
    {
        std::exit(-12);
    }
}

std::int32_t memory::read_instr_byte(int address)
{
    return (unsigned char)instructions[address - ADDR_INSTR];
}

std::int32_t memory::load_byte(int address, char format) const//TESTED
{
    // Check address range
    if((address >= ADDR_DATA)&&(address < ADDR_DATA + DATA_SIZE))
    {
        if(stored_data.find(address) != stored_data.end())
        {
            if(format == 'u')
            {
                return (unsigned char)(stored_data.at(address));
            }
            else if(format == 's')
            {
                return stored_data.at(address);
            }
            else
            {
                std::exit(-20);
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        std::exit(-11);
    }
}


std::int32_t memory::load_half_word(int address, char format) const//TESTED
{
    // Check address range
    if(((address >= ADDR_DATA)&&((address + 1) < ADDR_DATA + DATA_SIZE))||((address >= ADDR_INSTR)&&((address + 1) < ADDR_INSTR + INSTR_SIZE)))
    {
        //Check half word alignment
        if(address%2 == 0)
        {
            std::int32_t word = ((load_byte(address, 'u'))<<8)|(load_byte(address+1, 'u'));
            if(format == 'u')
            {
                return word;
            }
            else if(format == 's')
            {
                if(word >> 15 == 1)
                {
                    return word|(0xFFFF0000);
                }
                else if(word >> 15 == 0)
                {
                    return word;
                }
                else
                {
                    std::exit(-20);
                }
            }
            else
            {
                std::exit(-20);
            }
        }
        else
        {
          std::exit(-11);
        }
    }
    else
    {
        std::exit(-11);
    }
}


std::int32_t memory::load_word(int address) const//TESTED
{
    // Range check
    if(((address >= ADDR_DATA)&&((address + 3) < ADDR_DATA + DATA_SIZE))||((address >= ADDR_INSTR)&&((address + 3) < ADDR_INSTR + INSTR_SIZE)))
    {
        // Alignment check
        if(address%4 == 0)
        {
            std::int32_t word = ((load_byte(address, 'u'))<<24)|((load_byte(address+1, 'u'))<<16)|((load_byte(address+2, 'u'))<<8)|(load_byte(address+3, 'u'));
            return word;
        }
        else
        {
            std::exit(-11);
        }
    }
    else
    {
        std::exit(-11);
    }
}


std::int32_t memory::load_word_left(int address, std::int32_t register_value) const// TESTED
{
    int offset = 3 - (address%4);
    // Range check
    if(((address >= ADDR_DATA)&&((address + offset) < ADDR_DATA + DATA_SIZE))||((address >= ADDR_INSTR)&&((address + offset) < ADDR_INSTR + INSTR_SIZE)))
    {
        int shift = 24;
        int word = 0;
        int mask = 0xFFFFFFFF;
        int i = 0;
        while(offset >= 0)
        {
            word = word|(load_byte(address + i, 'u')<<shift);
            shift = shift - 8;
            offset = offset - 1;
            mask = (mask>>8)&(0x00FFFFFF);
            i++;
        }
        register_value = register_value&mask;
        register_value = word|register_value;
        return register_value;
    }
    else
    {
        std::exit(-11);
    }
}


std::int32_t memory::load_word_right(int address, std::int32_t register_value) const// TESTED
{
    int offset = address%4;
    if(((address >= ADDR_DATA)&&((address + (3-offset)) < ADDR_DATA + DATA_SIZE))||((address >= ADDR_INSTR)&&((address + (3-offset)) < ADDR_INSTR + INSTR_SIZE)))
    {
        int shift = 0;
        int word = 0;
        int i = 0;
        int mask = 0xFFFFFFFF;
        while(offset >= 0)
        {
            word = word|(load_byte(address - i, 'u')<<shift);
            shift = shift + 8;
            offset = offset - 1;
            mask = (mask<<8)&(0xFFFFFF00);
            i++;
        }
        register_value = register_value&mask;
        register_value = register_value|word;
        return register_value;
    }
    else
    {
        std::exit(-11);
    }
}


int memory::store_byte(int address, std::int32_t word)//TESTED - stores byte
{
    // range check
    if((address >= ADDR_DATA)&&(address < ADDR_DATA + DATA_SIZE))
    {
        char byte = word&(0b11111111);
        stored_data[address] = byte;
        return 0;
    }
    else
    {
        std::exit(-11);
    }
}


int memory::store_half_word(int address, std::int32_t word)//TESTED
  {
    // alignment check
    if(address%2 == 0)
    {
        // Range check
        if((address >= ADDR_DATA)&&((address+1) < ADDR_DATA + DATA_SIZE))
        {
            store_byte((address), word>>8);
            store_byte((address+1), word);
            return 0;
        }
        else
        {
            std::exit(-11);
        }
    }
    else
    {
        std::exit(-11);
    }
}


int memory::store_word(int address, std::int32_t word)//TESTED - stores word
{
    // Alignment check
    if(address%4 == 0)
    {
      // Range check
      if((address >= ADDR_DATA)&&((address+3) < ADDR_DATA + DATA_SIZE))
      {
          store_byte(address, word>>24);
          store_byte((address+1), word>>16);
          store_byte((address+2), word>>8);
          store_byte((address+3), word);
          return 0;
      }
      else
      {
          std::exit(-11);
      }
  }
  else
  {
      std::exit(-11);
  }
}


int memory::GETCreadbyte()//memory mapped input - TESTED
{
    GETCmemory[3] = std::getchar();
    if(GETCmemory[3] == EOF)
    {
        return(-1);
    }
    else
    {
        return (unsigned int)GETCmemory[3];
    }
}


int memory::PUTCwritebyte(std::int32_t word)//memory mapped output - TESTED
  {
    PUTCmemory[0] = (word >> 24)&0xFF;
    PUTCmemory[1] = (word >> 16)&0xFF;
    PUTCmemory[2] = (word >> 8)&0xFF;
    PUTCmemory[3] = word&0xFF;
    if(std::putchar(PUTCmemory[3]) != EOF)
    {
        return 0;
    }
    else
    {
        std::exit(-21);
    }
}


void memory::advancePC(int newNPC)
{
    if(NPC%4 == 0)
    {
        if((NPC >= ADDR_INSTR)&&(NPC < ADDR_INSTR + INSTR_SIZE))
        {
            PC = NPC;
            NPC = newNPC;
        }
        else if(NPC == 0)
        {
            PC = NPC;
        }
        else
        {
            std::exit(-11);
        }
    }
    else
    {
      std::exit(-11);
    }
}


int memory::getPC()//TESTED
{
    return PC;
}


int memory::getNPC()
{
    return NPC;
}
