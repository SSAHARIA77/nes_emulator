#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include <map>
//Forward Declaration
class Bus; // As bus is also dependent on cpu 

class CPU6502{
    public:
        CPU6502();
        ~CPU6502();

    private:
        Bus * bus = nullptr;
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr);

    public:
    void connectBus(Bus *b){bus = b;};

    //Processor Registers
    uint8_t a = 0x00;
    uint8_t x = 0x00;
    uint8_t y = 0x00;
    uint16_t pc = 0x0000;
    uint8_t pStatus = 0x00;
    uint8_t sp = 0x00;


    //External Event Functions to produce a change in the state
    //These three functions need to work asynchronously and should respond anytime they are called
    void reset();
    void irq(); //Interrupt Request
    void nonMaskableIrq(); //Interrupt Request that cannot be disabl    ed


    void clock(); //Perform one clock cycle worth of update

    bool complete();

    std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);


    enum FLAGS6502{
        C = (1 << 0),   //Carry Flag
        Z = (1 << 1),   //Zero Flag
        I = (1 << 2),   //Interrupt Disable Flag
        D = (1 << 3),   //Decimal Mode Flag (Not using in our emulator)
        B = (1 << 4),   //Break Flag
        U = (1 << 5),   //Unused Flag
        V = (1 << 6),   // Overflow Flag
        N = (1 << 7)    // Negative Flag
    };


    private:
    //To handle status register
    uint8_t getFlag(FLAGS6502 f);
    void setFlag(FLAGS6502 f, bool v);

    //Other variables to handle emulation
    uint8_t fetched = 0x00;  //Represents the working input value of the ALU
    uint16_t temp = 0x00;
    uint16_t addrAbs = 0x0000; //Represents the absolute address of the operand for the data 
    uint16_t addrRel = 0x00; //For branch instruction to store the relative address   
    uint8_t opcode = 0x00;  //Instruction Byte
    uint32_t clockCount = 0;    //Total Clock Cycles
    uint8_t cycles = 0; //Counts the number of clock cycles remaining for the instruction


    //Addressing Modes
    uint8_t IMP();	uint8_t IMM();	
	uint8_t ZP0();	uint8_t ZPX();	
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();	
	uint8_t ABY();	uint8_t IND();	
	uint8_t IZX();	uint8_t IZY();

    //Opcodes
    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    // To handle unofficial opcode
	// functionally identical to a NOP
    uint8_t XXX();

    uint8_t fetch();

    struct Instruction{
        std::string name; //Just for disassembler
        uint8_t (CPU6502::*operate)(void) = nullptr;
        uint8_t (CPU6502::*addrMode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    //To store the complete instruction set with the combination of all possible functions and addressing modes
    std::vector<Instruction> lookup;

};