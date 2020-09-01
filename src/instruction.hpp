#include <iostream>
#include "memory.hpp"

// Class that defines the various processor instructions.

class instructions{
public:
    // Constructor, no need for destructor.
    instructions(memory& a_in);

    // Obtains value in register.
    std::int32_t get_reg_value(int reg_number) const;

    //Sets value in register
    void set_reg_value(int reg_number, std::int32_t value);

    //Simple getter method that returns value of the branch member variable.
    bool did_it_branch() const;

    // Sets the below member variables based on binary instruction provided (see implementation)
    void set_r_data(int rs_in , int rt_in , int rd_in , int shift_in);
    void set_i_data(int rs_in , int rt_in , int16_t imm);

    // r type

    void add();

    void addu();

    void andop();

    void div();

    void divu();

    void mfhi();

    void mflo();

    void mthi();

    void mtlo();

    void mult();

    void multu();

    void norop();

    void orop();

    void sll();

    void sllv();

    void slt();

    void sltu();

    void sra();

    void srav();

    void srl();

    void srlv();

    void sub();

    void subu();

    void xorop();



    //I type

    void addi();

    void addiu();

    void andi();

    void beq_check();

    void bgez_check();

    void bgezal_check_and_link(int PC_value);

    void bgtz_check();

    void blez_check();

    void bltz_check();

    void bltzal_check_and_link(int PC_value);

    void bne_check();

    void lb();

    void lbu();

    void lh();

    void lhu();

    void lui();

    void lw();

    void lwl();

    void lwr();

    void ori();

    void sb();

    void sh();

    void slti();

    void sltiu();

    void sw();

    void xori();


    //J type
    void jal_link(int PC_value);


private:
    memory a;

    //These would ordinarily be defined by the various bit fields in a 32 bit R-type instruction.
    int rs = 0;
    int rt = 0; // rt
    int rdest = 0; //rd
    int shift = 0;
    int funct = 0;

    // This is the 16 bit field in an I type instruction
    std::int16_t imm = 0;

    bool branch = false;

    //The 32 general purpose registers.
    std::vector<std::int32_t> reg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // The HI and LO registers that store results of multiplication, division and modulus.
    std::uint32_t regHI = 0;
    std::uint32_t regLO = 0;
};
