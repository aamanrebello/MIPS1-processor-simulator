#include<string>
#include<iostream>
#include<cstdlib>
#include "instruction.hpp"
int extract_field(int shift, int width, std::int32_t instruction);
void decodeR(int& sourcereg1 , int& sourcereg2 , int& destreg, int& shamt , std::int32_t instruction);
void decodeI(int& sourcereg1 , int& sourcereg2 , std::int16_t& immediate, std::int32_t instruction);
bool instr_execute(std::int32_t instruction, memory& obj, instructions& instr_obj);




int extract_field(int shift, int width, std::int32_t instruction)//extracts field from instruction
{
    int extractor = 0;
    while(width > 0)
    {
      extractor = extractor*2 + 1;
      width--;
    }
    return (unsigned int)((instruction>>shift)&extractor);
}


void decodeR(int& sourcereg1 , int& sourcereg2 , int& destreg, int& shamt , std::int32_t instruction)
{
    sourcereg1 = extract_field(21, 5, instruction);
    sourcereg2 = extract_field(16, 5, instruction);
    destreg = extract_field(11, 5, instruction);
    shamt = extract_field(6, 5, instruction);
}

void decodeI(int& sourcereg1 , int& sourcereg2 , std::int16_t& immediate, std::int32_t instruction)
{
    sourcereg1 = extract_field(21, 5, instruction);
    sourcereg2 = extract_field(16, 5, instruction);
    immediate = extract_field(0, 16, instruction);
}


bool instr_execute(std::int32_t instruction, memory& obj, instructions& instr_obj)
{
    int newNPC = obj.getNPC() + 4;
    bool executed = false;
    int opcode = (instruction>>26)&(0b111111);//determine opcode
    if(opcode == 0)
    {
          int rs = 0;
          int rt = 0;
          int destreg = 0;
          int shamt = 0;
          int funct = instruction&(0b111111);//determine function code
          decodeR(rs, rt, destreg, shamt, instruction);
          instr_obj.set_r_data(rs, rt, destreg, shamt);
          // Determines what instruction to do based on the different fields
          switch(funct)
          {
              case 0:
              {
                  instr_obj.sll();
                  executed = true;
                  break;
              }
              case 2:
              {
                  instr_obj.srl();
                  executed = true;
                  break;
              }
              case 3:
              {
                  instr_obj.sra();
                  executed = true;
                  break;
              }
              case 4:
              {
                  instr_obj.sllv();
                  executed = true;
                  break;
              }
              case 6:
              {
                  instr_obj.srlv();
                  executed = true;
                  break;
              }
              case 7:
              {
                  instr_obj.srav();
                  executed = true;
                  break;
              }
              case 8://jr
              {
                  std::int32_t target = instr_obj.get_reg_value(rs);
                  newNPC = (int)target;
                  executed = true;
                  break;
              }
              case 9://jalr
              {
                  newNPC = instr_obj.get_reg_value(rs);
                  std::int32_t return_addr = obj.getPC() + 8;
                  instr_obj.set_reg_value(destreg, return_addr);
                  executed = true;
                  break;
              }
              case 12:
              {
                  //std::cerr << "Instruction functionality does not exist" << std::endl;
                  std::exit(-21);
                  break;
              }
              case 13:
              {
                  //std::cerr << "Instruction functionality does not exist" << std::endl;
                  std::exit(-21);
                  break;
              }
              case 16:
              {
                  instr_obj.mfhi();
                  executed = true;
                  break;
              }
              case 17:
              {
                  instr_obj.mthi();
                  executed = true;
                  break;
              }
              case 18:
              {
                  instr_obj.mflo();
                  executed = true;
                  break;
              }
              case 19:
              {
                  instr_obj.mtlo();
                  executed = true;
                  break;
              }
              case 24:
              {
                  instr_obj.mult();
                  executed = true;
                  break;
              }
              case 25:
              {
                  instr_obj.multu();
                  executed = true;
                  break;
              }
              case 26:
              {
                  instr_obj.div();
                  executed = true;
                  break;
              }
              case 27:
              {
                  instr_obj.divu();
                  executed = true;
                  break;
              }
              case 32:
              {
                  instr_obj.add();
                  executed = true;
                  break;
              }
              case 33:
              {
                  instr_obj.addu();
                  executed = true;
                  break;
              }
              case 34:
              {
                  instr_obj.sub();
                  executed = true;
                  break;
              }
              case 35:
              {
                  instr_obj.subu();
                  executed = true;
                  break;
              }
              case 36:
              {
                  instr_obj.andop();
                  executed = true;
                  break;
              }
              case 37:
              {
                  instr_obj.orop();
                  executed = true;
                  break;
              }
              case 38:
              {
                  instr_obj.xorop();
                  executed = true;
                  break;
              }
              case 39:
              {
                  instr_obj.norop();
                  executed = true;
                  break;
              }
              case 42:
              {
                  instr_obj.slt();
                  executed = true;
                  break;
              }
              case 43:
              {
                  instr_obj.sltu();
                  executed = true;
                  break;
              }
              default:
                  //std::cerr << "Instruction does not exist\n";
                  std::exit(-12);
        }
    }

    else if(((opcode >= 32)&&(opcode <= 46))||((opcode >= 4)&&(opcode <= 15))||(opcode == 1))
    {
        int rs = 0;
        int rt = 0;
        std::int16_t immediate = 0;
        decodeI(rs , rt , immediate, instruction);
        instr_obj.set_i_data(rs, rt, immediate);
        switch(opcode)
        {
            case 1:
            {
                switch(rt)
                {
                    case 0:
                    {
                        instr_obj.bltz_check();
                        std::int32_t imm = immediate;
                        if(instr_obj.did_it_branch())
                        {
                            newNPC = obj.getNPC() + (imm<<2);
                        }
                        executed = true;
                        break;
                    }
                    case 1:
                    {
                        instr_obj.bgez_check();
                        std::int32_t imm = immediate;
                        if(instr_obj.did_it_branch())
                        {
                            newNPC = obj.getNPC() + (imm<<2);
                        }
                        executed = true;
                        break;
                    }
                    case 16:
                    {
                        instr_obj.bltzal_check_and_link(obj.getPC());
                        std::int32_t imm = immediate;
                        if(instr_obj.did_it_branch())
                        {
                            newNPC = obj.getNPC() + (imm<<2);
                        }
                        executed = true;
                        break;
                    }
                    case 17:
                    {

                        instr_obj.bgezal_check_and_link(obj.getPC());
                        std::int32_t imm = immediate;
                        if(instr_obj.did_it_branch())
                        {
                            newNPC = obj.getNPC() + (imm<<2);
                        }
                        executed = true;
                        break;
                    }
                    default:
                        //std::cerr << "Instruction does not exist\n";
                        std::exit(-12);
                }
                break;
            }

            case 4:
            {
                instr_obj.beq_check();
                std::int32_t imm = immediate;
                if(instr_obj.did_it_branch())
                {
                    newNPC = obj.getNPC() + (imm<<2);
                }
                executed = true;
                break;
            }
            case 5:
            {
                instr_obj.bne_check();
                std::int32_t imm = immediate;
                if(instr_obj.did_it_branch())
                {
                    newNPC = obj.getNPC() + (imm<<2);
                }
                executed = true;
                break;
            }
            case 6:
            {
                instr_obj.blez_check();
                std::int32_t imm = immediate;
                if(instr_obj.did_it_branch())
                {
                    newNPC = obj.getNPC() + (imm<<2);
                }
                executed = true;
                break;
            }
            case 7:
            {
                instr_obj.bgtz_check();
                std::int32_t imm = immediate;
                if(instr_obj.did_it_branch())
                {
                    newNPC = obj.getNPC() + (imm<<2);
                }
                executed = true;
                break;
            }
            case 8:
            {
                instr_obj.addi();
                executed = true;
                break;
            }
            case 9:
            {
                instr_obj.addiu();
                executed = true;
                break;
            }
            case 10:
            {
                instr_obj.slti();
                executed = true;
                break;
            }
            case 11:
            {
                instr_obj.sltiu();
                executed = true;
                break;
            }
            case 12:
            {
                instr_obj.andi();
                executed = true;
                break;
            }
            case 13:
            {
                instr_obj.ori();
                executed = true;
                break;
            }
            case 14:
            {
                instr_obj.xori();
                executed = true;
                break;
            }
            case 15:
            {
                instr_obj.lui();
                executed = true;
                break;
            }
            case 17:
            {
                //std::cerr << "Instruction functionality does not exist" << std::endl;
                std::exit(-21);
            }
            case 32:
            {
                std::int32_t imm32 = immediate;
                std::int32_t address = instr_obj.get_reg_value(rs) + imm32;
                // Check if required memory address corresponds to instruction memory read.
                if((address >= ADDR_INSTR)&&(address < ADDR_INSTR + INSTR_SIZE))
                {
                  if(address < ADDR_INSTR + obj.get_instr_size())
                  {
                    std::int32_t a = obj.read_instr_byte(address);
                    // Determine how we will sign extend to full 32 bits.
                    if((a >> 7)!=0)
                    {
                      instr_obj.set_reg_value(rt, (obj.read_instr_byte(address))|(0xFFFFFF00));
                    }
                    else
                    {
                      instr_obj.set_reg_value(rt, obj.read_instr_byte(address));
                    }
                  }
                  // Else we have a normal data memory read.
                  else
                  {
                    instr_obj.set_reg_value(rt, 0);
                  }
                }
                else
                {
                    instr_obj.lb();
                }
                executed = true;
                break;
            }
            case 33:
            {
                std::int32_t imm32 = immediate;
                std::int32_t address = instr_obj.get_reg_value(rs) + imm32;
                // Check if required memory address corresponds to instruction memory half word read.
                if(((address >= ADDR_INSTR)&&(address+1 < ADDR_INSTR + INSTR_SIZE))&&(address%2 == 0))
                {
                    if(address + 1 < ADDR_INSTR + obj.get_instr_size())
                    {
                      std::int32_t a = ((obj.read_instr_byte(address)<<8)|(obj.read_instr_byte(address + 1)));
                      // Determine how we will sign extend to full 32 bits.
                      if((a >> 15) != 0)
                      {
                        instr_obj.set_reg_value(rt, (0xFFFF0000)|a);
                      }
                      else
                      {
                        instr_obj.set_reg_value(rt, a);
                      }
                    }
                    else
                    {
                      instr_obj.set_reg_value(rt, 0);
                    }
                }
                // Else we have a normal data memory read.
                else
                {
                  instr_obj.lh();
                }
                executed = true;
                break;
            }
            case 34:
            {
                // TODO: Implement instruction memory read.
                instr_obj.lwl();
                executed = true;
                break;
            }
            case 35:
            {
                std::int32_t imm32 = immediate;
                std::int32_t address = instr_obj.get_reg_value(rs) + imm32;
                // Check if required memory address corresponds to instruction memory word read.
                if(((address >= ADDR_INSTR)&&(address+3 < ADDR_INSTR + INSTR_SIZE))&&(address%4 == 0))
                {
                  if(address + 3 < ADDR_INSTR + obj.get_instr_size())
                  {
                    std::int32_t a = ((obj.read_instr_byte(address)<<24)|((obj.read_instr_byte(address + 1))<<16)|((obj.read_instr_byte(address + 2))<<8)|(obj.read_instr_byte(address + 3)));
                    instr_obj.set_reg_value(rt, a);
                  }
                  else
                  {
                    instr_obj.set_reg_value(rt, 0);
                  }
                }
                // Else we have a normal data memory read.
                else
                {
                  instr_obj.lw();
                }
                executed = true;
                break;
            }
            case 36:
            {
                std::int32_t imm32 = immediate;
                std::int32_t address = instr_obj.get_reg_value(rs) + imm32;
                // Check if required memory address corresponds to instruction memory word read.
                if((address >= ADDR_INSTR)&&(address < ADDR_INSTR + INSTR_SIZE))
                {
                  if(address < ADDR_INSTR + obj.get_instr_size())
                  {
                    instr_obj.set_reg_value(rt, obj.read_instr_byte(address));
                  }
                  else
                  {
                    instr_obj.set_reg_value(rt, 0);
                  }
                }
                // Else we have a normal data memory read.
                else
                {
                    instr_obj.lbu();
                }
                executed = true;
                break;
            }
            case 37:
            {
              std::int32_t imm32 = immediate;
              std::int32_t address = instr_obj.get_reg_value(rs) + imm32;
              // Check if required memory address corresponds to instruction memory word read.
              if(((address >= ADDR_INSTR)&&(address+1 < ADDR_INSTR + INSTR_SIZE))&&(address%2 == 0))
              {
                  if(address + 1 < ADDR_INSTR + obj.get_instr_size())
                  {
                    std::int32_t a = ((obj.read_instr_byte(address)<<8)|(obj.read_instr_byte(address + 1)));
                    instr_obj.set_reg_value(rt, a);
                  }
                  else
                  {
                    instr_obj.set_reg_value(rt, 0);
                  }
              }
              // Else we have a normal data memory read.
              else
              {
                instr_obj.lhu();
              }
              executed = true;
              break;
            }
            case 38:
            {
                instr_obj.lwr();
                executed = true;
                break;
            }
            case 40:
            {
                instr_obj.sb();
                executed = true;
                break;
            }
            case 41:
            {
                instr_obj.sh();
                executed = true;
                break;
            }
            case 42:
            {
                //std::cerr << "Instruction functionality does not exist" << std::endl;
                std::exit(-21);
            }
            case 43:
            {
                instr_obj.sw();
                executed = true;
                break;
            }
            case 46:
            {
                //std::cerr << "Instruction functionality does not exist" << std::endl;
                std::exit(-21);
            }
            default:
                //std::cerr << "Unexpected error" << std::endl;
                std::exit(-20);
        }
    }


    else if((opcode == 2)||(opcode == 3))
    {
        int target = (unsigned int)(instruction&(0x3FFFFFF));
        if(opcode == 2)//j
        {
            int address = ((obj.getPC()+4)&(0xF0000000))|(target << 2);
            newNPC = address;
            executed = true;
        }
        else if(opcode == 3)//jal
        {
            instr_obj.jal_link(obj.getPC());
            int address = ((obj.getPC()+4)&(0xF0000000))|(target << 2);
            newNPC = address;
            executed = true;
        }
        else
        {
            //std::cerr << "Unexpected error" << std::endl;
            std::exit(-20);
        }
      }

      else
      {
          //std::cerr << "Instruction does not exist\n";
          std::exit(-12);
      }

      obj.advancePC(newNPC);
      if(obj.getPC() == 0)
      {
          int retcode = (instr_obj.get_reg_value(2))&(0xFF);
          std::exit(retcode);
      }
      // Returning true indicates that the instruction executed successfully.
      return executed;
}



int main(int argc, char *argv[])
{
    memory obj;
    instructions instr_obj(obj);
    if(argc>1)//if a filename has been passed
    {
        if(obj.read_and_store_instructions(argv[1]) == true)// argv1 is filename
        {
            //test(obj, instr_obj, argv[1]);
            while(obj.getPC() < ADDR_INSTR + INSTR_SIZE - 1)//remain within instruction array size
            {
                std::int32_t execute = obj.load_instruction();
                if(execute != (-1))
                {
                    if(instr_execute(execute, obj, instr_obj) != true)
                    {
                      //std::cerr << "Unexpected error" << std::endl;
                      std::exit(-20);
                    }
                }
                else
                {
                    //std::cerr << "ERROR LOADING INSTRUCTION\n";
                    std::exit(-21);
                }
            }
        }
        else
        {
            std::exit(-21);
            //std::cerr << "ERROR IN READING/STORING INSTRUCTIONS\n";
        }
    }
    else
    {
        //std::cerr << "no filename provided\n";
        std::exit(-21);
    }
    return 0;
}
