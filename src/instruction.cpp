#include <cstdlib>
#include "instruction.hpp"

instructions::instructions(memory& a_in)
{
  a=a_in;
}


std::int32_t instructions::get_reg_value(int reg_number) const//gives register value
{
    if((reg_number<0) || (reg_number>31))
    {
        std::exit(-20);
    }
    return reg[reg_number];

}

void instructions::set_reg_value(int reg_number, std::int32_t value)
{
    if((reg_number<=0) || (reg_number>31))
    {
        std::exit(-20);
    }
    reg[reg_number] = value;
}

void instructions::set_r_data(int rs_in , int rt_in , int rdest_in , int shift_in)
{
    if(((rs_in>31)||(rt_in>31)||(rdest_in>31)||(shift_in > 31))||((rs_in<0)||(rt_in<0)||(rdest_in<0)||(shift_in<0)))
    {
        std::exit(-20);
    }
    rs = rs_in;
    rt = rt_in;
    rdest = rdest_in;
    shift = shift_in;
}


void instructions::set_i_data(int rs_in , int rt_in , int16_t imm_in)
{
    if(((rs_in > 31)||(rt_in > 31))||((rs_in < 0)||(rt_in < 0)))
    {
        std::exit(-20);
    }
    rs = rs_in;
    rt = rt_in;
    imm = imm_in;
}


bool instructions::did_it_branch() const
{
    return branch;
}


//r type
void instructions::add()//add - fine
{
    if(rdest != 0)
    {

        std::int32_t sum = reg[rs] + reg[rt];
        // Check for an overflow
        if((reg[rs]>0) && (reg[rt] > 0) && (sum <= 0))
        {
            std::exit(-10);
        }
        else if((reg[rs]<0) && (reg[rt] < 0) && (sum >= 0))
        {
            std::exit(-10);
        }
        else
        {
            reg[rdest] = sum;
        }
    }
}

void instructions::addu()//unsigned add - fine
{
    if(rdest != 0)
    {
        std::uint32_t r1 = reg[rs];
        std::uint32_t r2 = reg[rt];
        reg[rdest] = (std::uint32_t)(r1+r2);
    }
}


void instructions::andop()//and - fine
{
    if(rdest != 0)
    {
        reg[rdest] = reg[rs] & reg[rt];
    }
}


void instructions::div()
{
    std::int32_t r1 = reg[rs];
    std::int32_t r2 = reg[rt];
    if(r2 != 0)
    {
      if(!((r1 == 0x80000000)&&(r2 == (-1))))
      {
        regLO = r1/r2;
        regHI = r1%r2;
      }
    }
}

void instructions::divu()
{
       std::uint32_t r1 = reg[rs];
       std::uint32_t r2 = reg[rt];
       if(r2 != 0)
       {
            regLO = r1/r2;
            regHI = r1%r2;
       }
}

void instructions::mfhi()
{
    if(rdest != 0)
    {
          reg[rdest] = regHI;
    }
}

void instructions::mflo()
{
    if(rdest != 0)
    {
        reg[rdest] = regLO;
    }
}

void instructions::mthi()
{
    regHI = reg[rs];
}

void instructions::mtlo()
{
    regLO =  reg[rs];
}

void instructions::mult()
{
    std::int64_t r1 = (std::int32_t)reg[rs];
    std::int64_t r2 = (std::int32_t)reg[rt];
    std::int64_t c = r1*r2;
    regLO = c&(0xFFFFFFFF);
    regHI = (c>>32)&0xFFFFFFFF;
}

void instructions::multu()
{
    std::uint64_t r1 = (std::uint32_t)reg[rs];
    std::uint64_t r2 = (std::uint32_t)reg[rt];
    std::uint64_t c = r1*r2;
    regLO = c&(0xFFFFFFFF);
    regHI = (c>>32)&0xFFFFFFFF;
}

void instructions::norop()
{
    if(rdest != 0)
    {
        reg[rdest] =  ~(reg[rs] | reg[rt]);
    }
}

void instructions::orop()//done
{
    if(rdest != 0)
    {
        reg[rdest] =  reg[rs] | reg[rt];
    }
}

void instructions::sll()//done
{
    if(rdest != 0)
    {
        reg[rdest] = reg[rt] << shift;
    }
}

void instructions::sllv()
{
    if(rdest != 0)
    {
      std::uint32_t shift_sslv = reg[rs]&(0b11111);
      reg[rdest] = reg[rt] << shift_sslv;
    }

}

void instructions::slt()
{
    if(rdest != 0)
    {
        if(reg[rs] < reg[rt])
        {
            reg[rdest] = 1;
        }
        else
        {
            reg[rdest] = 0;
        }
    }
}


void instructions::sltu()
{
    if(rdest != 0)
    {
        std::uint32_t reg_rs = reg[rs];
        std::uint32_t reg_rt = reg[rt];
        if(reg_rs < reg_rt)
        {
            reg[rdest] = 1;
        }
        else
        {
            reg[rdest] = 0;
        }
    }
}

void instructions::sra()
{
    if(rdest != 0)
    {
        std::int32_t n = reg[rt];
        if((n>>31) != 0)
        {
            std::int32_t mask = 0xFFFFFFFF;
            mask = mask << (32 - shift);
            reg[rdest] = (reg[rt] >> shift)|mask;
        }
        else
        {
            reg[rdest] = reg[rt] >> shift;
        }
    }
}

void instructions::srav()
{
    std::uint32_t shift_srav = reg[rs]&(0b11111);
    if(rdest != 0)
    {
        std::int32_t n = reg[rt];
        if((n>>31) != 0)
        {
            std::int32_t mask = 0xFFFFFFFF;
            mask = mask << (32 - shift_srav);
            reg[rdest] = (reg[rt] >> shift_srav)|mask;
        }
        else
        {
            reg[rdest] = reg[rt] >> shift_srav;
        }
    }
}

void instructions::srl()
{
    if(rdest != 0)
    {
        std::uint32_t reg_rt = reg[rt];
        reg[rdest] = reg_rt >> shift;
    }
}

void instructions::srlv()
{
    if(rdest != 0)
    {
        std::uint32_t reg_rs = reg[rs]&(0b11111);
        std::uint32_t reg_rt = reg[rt];
        reg[rdest] = reg_rt>> reg_rs;
    }
}

void instructions::sub()
{
    if(rdest != 0)
    {
        std::int32_t result = reg[rs] - reg[rt];
        if (((reg[rs] >= 0) && (reg[rt] < 0) && (result < 0)) || ((reg[rs] < 0) && (reg[rt] >= 0) && (result >= 0)))
        {   //checks for overflow + underflow
            std::exit(-10);
        }
        reg[rdest] = result;
    }

}

void instructions::subu()
{
    if(rdest != 0)
    {
        std::uint32_t reg_rs = reg[rs];
        std::uint32_t reg_rt = reg[rt];
        reg[rdest] = reg_rs - reg_rt;
    }
}

void instructions::xorop()
{
    if(rdest != 0)
    {
        reg[rdest] =  reg[rs] ^ reg[rt];
    }
}

//I type


void instructions::addi()
{
    if(rt != 0)
    {
        std::int32_t imm32 = imm;
        std::int32_t sum = reg[rs] + imm32;
        //Checking for overflow
        if( (reg[rs]>0) && (imm32 > 0) && (sum < 0))
        {
            std::exit(-10);
        }
        else if( (reg[rs]<0) && (imm32 < 0) && (sum > 0) )
        {
            std::exit(-10);
        }
        else
        {
            reg[rt] = sum;
        }
    }
}

void instructions::addiu()
{
    if(rt != 0)
    {
        std::int32_t imm32 = imm;
        std::int32_t sum = reg[rs] + imm32;
        reg[rt] = sum;
    }
}


void instructions::andi()
{
    if(rt != 0)
    {
        std::int32_t imm32 = ((std::int32_t)imm)&(0x0000FFFF);
        reg[rt] = reg[rs] & imm32;
    }
}

void instructions::beq_check()
{
      if(reg[rs] == reg[rt])
      {
          branch = true;
      }
      else
      {
          branch = false;
      }
}

void instructions::bgez_check()
{
       if(reg[rs] >= reg[0])
       {
            branch = true;
       }
       else
       {
            branch = false;
       }
}

void instructions::bgezal_check_and_link(int PC_value)
{
       if(rs == 31)
       {
         std::exit(-11);
       }
       if(reg[rs] >= reg[0])
       {
            reg[31] = PC_value + 8;
            branch = true;
       }
       else
       {
            branch = false;
       }
}


void instructions::bgtz_check()
{
       if(reg[rs] > reg[0])
       {
            branch = true;
       }
       else
       {
            branch = false;
       }
}


void instructions::blez_check()
{
       if(reg[rs] <= reg[0])
       {
            branch = true;
       }
       else
       {
            branch = false;
       }
}


void instructions::bltz_check()
{
       if(reg[rs] < reg[0])
       {
            branch = true;
       }
       else
       {
          branch = false;
       }
}


void instructions::bltzal_check_and_link(int PC_value)
{
       if(rs == 31)
       {
           std::exit(-11);
       }
       if(reg[rs] < reg[0])
       {
            reg[31] = PC_value + 8;
            branch = true;
       }
       else
       {
            branch = false;
       }
}


void instructions::bne_check()
{
       if(reg[rs] != reg[rt])
       {
            branch = true;
       }
       else
       {
          branch = false;
       }
}

void instructions::lb()
{
    std::int32_t imm32 = imm;
    std::int32_t address = reg[rs] + imm32;
    if((address == ADDR_GETC)||(address == ADDR_GETC + 1)||(address == ADDR_GETC + 2))
    {
        reg[rt] = 0;
    }
    else if(address == ADDR_GETC + 3)
    {
        std::int32_t rdout = a.GETCreadbyte();
        if((rdout >> 7)!=0)
        {
            reg[rt] = rdout|(0xFFFFFF00);
        }
        else
        {
            reg[rt] = rdout;
        }
    }
    else
    {
        reg[rt] = a.load_byte(address, 's');
    }
}

void instructions::lbu()
{
  std::int32_t imm32 = imm;
  std::int32_t address = reg[rs] + imm32;
  // Depending on the memory address we do different things.
  if((address == ADDR_GETC)||(address == ADDR_GETC + 1)||(address == ADDR_GETC + 2))
  {
      reg[rt] = 0;
  }
  else if(address == ADDR_GETC + 3)
  {
      reg[rt] = a.GETCreadbyte();
  }
  else
  {
      reg[rt] = a.load_byte(address, 'u');
  }
}

void instructions::lh()
{
    std::int32_t imm32 = imm;
    std::int32_t address = reg[rs] + imm32;
    // Depending on the memory address we do different things.
    if(address == ADDR_GETC)
    {
        reg[rt] = 0;
    }
    else if(address == ADDR_GETC + 2)
    {
        reg[rt] = a.GETCreadbyte();
    }
    else
    {
        reg[rt] = a.load_half_word(address, 's');
    }
}

void instructions::lhu()
{
    std::int32_t imm32 = imm;
    std::int32_t address = reg[rs] + imm32;
    // Depending on the memory address we do different things.
    if(address == ADDR_GETC)
    {
        reg[rt] = 0;
    }
    else if(address == ADDR_GETC + 2)
    {
        reg[rt] = a.GETCreadbyte();
    }
    else
    {
        reg[rt] = a.load_half_word(address, 'u');
    }
}

void instructions::lui()
{
    std::int32_t imm32 = imm;
    reg[rt] = (imm32<<16)&(0xFFFF0000);
}

void instructions::lw()
{
    std::int32_t imm32 = imm;
    std::uint32_t address = reg[rs] + imm32;
    if(address == ADDR_GETC)
    {
        reg[rt] = a.GETCreadbyte();
    }
    else
    {
        reg[rt] = a.load_word(address);
    }
}

void instructions::lwl()
{
    std::int32_t imm32 = imm;
    std::uint32_t address = reg[rs] + imm32;
    std::int32_t regvalue = reg[rt];
    reg[rt] = a.load_word_left(address, regvalue);
}

void instructions::lwr()
{
    std::int32_t imm32 = imm;
    std::uint32_t address = reg[rs] + imm32;
    std::int32_t regvalue = reg[rt];
    reg[rt] = a.load_word_right(address, regvalue);
}

void instructions::ori()
{
    if(rt != 0)
    {
        std::int32_t imm32 = ((std::int32_t)imm)&(0x0000FFFF);
        reg[rt] = reg[rs] | imm32;
    }
}

void instructions::sb()
{
    std::int32_t imm32 = imm;
    std::int32_t address = reg[rs] + imm32;
    std::int32_t word = reg[rt];
    // Depending on the memory address we do different things.
    if((address == ADDR_PUTC)||(address == ADDR_PUTC + 1)||(address == ADDR_PUTC + 2))
    {
        if(a.PUTCwritebyte(0) != 0)
        {
            std::exit(-20);
        }
    }
    else if(address == ADDR_PUTC + 3)
    {
        if(a.PUTCwritebyte(word) != 0)
        {
            std::exit(-20);
        }
    }
    else
    {
        if(a.store_byte(address, word) != 0)
        {
            std::exit(-20);
        }
    }
}

void instructions::sh()
{
    std::int32_t imm32 = imm;
    std::int32_t address = reg[rs] + imm32;
    std::int32_t word = reg[rt];
    // Depending on the memory address we do different things.
    if(address == ADDR_PUTC)
    {
        if(a.PUTCwritebyte(0) != 0)
        {
            std::exit(-20);
        }
    }
    else if(address == ADDR_PUTC + 2)
    {
        if(a.PUTCwritebyte(word) != 0)
        {
            std::exit(-20);
        }
    }
    else
    {
        if(a.store_half_word(address, word) != 0)
        {
            std::exit(-20);
        }
    }
}

void instructions::slti()
{
    if(rt != 0)
    {
        std::int32_t imm32 = imm;
        if(reg[rs] < imm32)
        {
            reg[rt] = 1;
        }
        else
        {
            reg[rt] = 0;
        }
    }
}


void instructions::sltiu()
{
    if(rt != 0)
    {
        std::int32_t imm32s = imm;
        uint32_t imm32 = imm32s;
        uint32_t reg_rs = reg[rs];

        if(reg_rs < imm32)
        {
            reg[rt] = 1;
        }
        else
        {
            reg[rt] = 0;
        }
    }
}

void instructions::sw()
{
    std::int32_t imm32 = imm;
    std::int32_t address = reg[rs] + imm32;
    std::int32_t word = reg[rt];
    if(address == ADDR_PUTC)
    {
        if(a.PUTCwritebyte(word) != 0)
        {
            std::exit(-20);
        }
    }
    else
    {
        if(a.store_word(address, word) != 0)
        {
            std::exit(-20);
        }
    }
}

void instructions::xori()
{
    if(rt != 0)
    {
        std::int32_t imm32 = ((std::int32_t)imm)&(0x0000FFFF);
        reg[rt] = reg[rs] ^ imm32;
    }
}

//J instructions

void instructions::jal_link(int PC_value)//not required
{
    reg[31] = PC_value +8;
}
