#include "CPU6502.h"
#include "Bus.h"

CPU6502::CPU6502(){
    using a = CPU6502; //Type definition for readability

    // Populating the Instruction Set
    // ??? represents invalid Instructions
    lookup = 
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU6502::~CPU6502(){

}


void CPU6502::write(uint16_t addr, uint8_t data){
    bus->cpuWrite(addr, data);
}

uint8_t CPU6502::read(uint16_t addr){
    return bus->cpuRead(addr, false);
}


//In the emulation we will perform the complete operation in one go. 
//In the actual hardware each step internally takes up certain number of clock cycles but dont care about that.
void CPU6502::clock(){

    if(cycles == 0){
        //Get the new instruction
        opcode = read(pc);

        setFlag(U, true);
        //Update the program Counter
        pc++;

        //Get the starting number of cycles for the instruction
        cycles = lookup.at(opcode).cycles;

        //Get Additional Cycles required

        //Addressing mode requires a clock cycle when page boundary is crossed.
        //This is because address is fetched as 2 bytes in NES and when offset of x or y register is added
        //Carry bit is set and this carry bit needs to be added to the high bit of the address which requires
        //additional cycle. But this additional cycle is required only for certain type of operations and not 
        //for operations like ASL. Hence are doing AND of these values to know if we really need the additional
        //cycle or not. For functions like ASL, their default cycles only contain the cycle for page crossing itself.
        uint8_t additionalCycles1 = (this->*lookup.at(opcode).addrMode)();
        uint8_t additionalCycles2 = (this->*lookup.at(opcode).operate)();

        //Note that our addrMode functions and operation functions returns only 0 or 1 as the result.
        //So we are adding 1 additional clock cycle if and only if both these functions returns 1.

        //Get the final number of cycles

        
        cycles += (additionalCycles1 & additionalCycles2);
        setFlag(U, true);
    }
    clockCount++;
    cycles--;
}

void CPU6502::irq(){
    if(getFlag(I) == 0){
        //PC and status register is stored in the stack when an interrupt occurs
        write(0x0100 + sp, (pc >> 8) & 0x00FF);
        sp--;
        write(0x0100 + sp, pc & 0x00FF);
        sp--;
        setFlag(B, 0);
        setFlag(U, 1);
        setFlag(I, 1);
        write(0x0100 + sp, pStatus);
        sp--;

        //Jump to a known location of interrupt which is hardcoded by the programmer
        addrAbs = 0xFFFE;
        uint16_t lo = read(addrAbs + 0);
        uint16_t hi = read(addrAbs + 1);
        pc = (hi << 8) | lo;

        cycles = 7;

    }
}

//Set the CPU to a known state
void CPU6502::reset(){
    //The nes cpu always look at this address to get the program
    //Programmer can hardcode his code's address to this addressx
    addrAbs = 0xFFFC;
    uint16_t lo = read(addrAbs + 0);
    uint16_t hi = read(addrAbs + 1);
    pc = (hi << 8) | lo;


    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    pStatus = 0x00 | U;


    addrRel = 0x0000;
    addrAbs = 0x0000;
    fetched = 0x00;
    
    //Interrupts and Resets takes cycles
    cycles = 8;
}

void CPU6502::nonMaskableIrq(){
        write(0x0100 + sp, (pc >> 8) & 0x00FF);
        sp--;
        write(0x0100 + sp, pc & 0x00FF);
        sp--;
        setFlag(B, 0);
        setFlag(U, 1);
        setFlag(I, 1);  
        write(0x0100 + sp, pStatus);
        sp--;

        addrAbs = 0xFFFA;
        uint16_t lo = read(addrAbs + 0);
        uint16_t hi = read(addrAbs + 1);
        pc = (hi << 8) | lo;

        cycles = 8;
}

bool CPU6502::complete(){
    return cycles == 0;
}

uint8_t CPU6502::XXX(){
    return 0;
}



uint8_t CPU6502::getFlag(FLAGS6502 f){
    return ((pStatus & f) > 0) ? 1 : 0;
}

void CPU6502::setFlag(FLAGS6502 f, bool v){
    //Toggle
    if(v){
        pStatus |= f;
    }else{
        pStatus &= ~f;
    }
}

//Addressing Modes

//Implicit Mode
uint8_t CPU6502::IMP(){
    //Implicit mode means accumulator can be used
    fetched = a;
    return 0;
}	

//Immediate Mode
uint8_t CPU6502::IMM(){
    //Get the data from the instruction itself
    //The next byte will basically be our operand
    //Note that we have incremented the pc as soon as the instruction is read in the clock function
    //Therefore we are using post increment as pc is already pointing to our data
    addrAbs = pc++;
    return 0;
}	

//Zero Page Addressing Mode

//Memory Address can be divided into pages and offset
//First 8 bit of address can represent page number and the lower 8 bit can represent the offset inside the page
//So the memory can be conceptually be considered as 256 pages of 256 bytes (offset)

//Zero Page mode means our data resides somewhere in the 0th page of the memory
//This is used to fast access this memory as we only use the offset and hence only 8 bit is required
uint8_t CPU6502::ZP0(){
    addrAbs = read(pc);
    pc++;
    addrAbs &= 0x00FF; //Masking the address to refer to the zeroeth page
    return 0;
}	

//Zero Page Addressing Mode with X offset

//Used for example in traversing an array with base address and x offset
uint8_t CPU6502::ZPX(){
    addrAbs = (read(pc) + x);
    pc++;
    addrAbs &= 0x00FF;
    return 0;
}	

//Zero Page Addressing Mode with Y offset
uint8_t CPU6502::ZPY(){
    addrAbs = (read(pc) + y);
    pc++;
    addrAbs &= 0x00FF;
    return 0;
}	

//Relative Addressing Mode
//For Branching Address
//Can't Jump Any further than 127 memory locations
uint8_t CPU6502::REL(){
    addrRel = read(pc);
    pc++;
    //To handle negative offset
    //If high bit of relative jump is 1 then it refers to negative offset
    if(addrRel & 0x80){
        addrRel |= 0xFF00;
    }
    return 0;
}

//Absolute Addressing Mode
uint8_t CPU6502::ABS(){
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addrAbs = (hi << 8) | lo;
    return 0;
}	

//Absolute Addressing Mode with X offset
uint8_t CPU6502::ABX(){
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addrAbs = (hi << 8) | lo;
    addrAbs += x;

    //Additional Cycle if we need to go to a new page
    //Page Boundary Crossing!
    if((addrAbs & 0xFF00) != (hi << 8)){
        return 1;
    }else{
        return 0;
    }
}	

//Absolute Addressing Mode with Y offset
uint8_t CPU6502::ABY(){
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addrAbs = (hi << 8) | lo;
    addrAbs += y;

    //Additional Cycle if we need to go to a new page
    if((addrAbs & 0xFF00) != (hi << 8)){
        return 1;
    }else{
        return 0;
    }
}	

//Indirect Addressing Mode
//Basically pc is having the address of the address of the data
//ADDRESS ==> ADDRESS OF DATA ==> ACTUAL DATA
uint8_t CPU6502::IND(){
    uint16_t ptrLo = read(pc);
    pc++;
    uint16_t ptrHi = read(pc);
    pc++;

    uint16_t ptr = (ptrHi << 8) | ptrLo;

    if(ptrLo == 0x00FF){
        // Simulate page boundary hardware bug
        /*
        For example:
            If ptr = 0x01FF, the CPU reads:
            Low byte from 0x01FF.
            High byte from 0x0100 (instead of 0x0200 which is basically the next page).
            So basically it wraps around the page instead of going to the new page.
            This bug is faithfully replicated in the emulator to maintain compatibility with software written for the 6502.
        */
        addrAbs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }else{
        //Behave Normally
        //Get the actual address of the data
        addrAbs = (read(ptr + 1) << 8) | read(ptr + 0);
    }


    return 0;
}	

// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// a location in page 0x00. The actual 16-bit address is read 
// from this location
uint8_t CPU6502::IZX(){
    uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addrAbs = (hi << 8) | lo;
	
	return 0;
}	

// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. From 
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes a
// change in page then an additional clock cycle is required.
uint8_t CPU6502::IZY(){
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addrAbs = (hi << 8) | lo;
	addrAbs += y;
	
	if ((addrAbs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}


//Fetch the data 
//We will call this function in the operations to get the operand

uint8_t CPU6502::fetch(){
    if(!(lookup.at(opcode).addrMode == &CPU6502::IMP)){
        fetched = read(addrAbs);
    }
    //If addressing mode is IMP then we have already set the fetched variable to the
    //value of the accumulator in the addressing mode function itself
    return fetched;
}


//Opcodes

uint8_t CPU6502::ADC(){
    //Grab the data into fetched variable
    fetch();

    //Carry bit is added to make it possible to work with multi byte numbers
    //Basically it can be used to store the higher byte if addition of lower bytes overflows
    temp = (uint16_t) a + (uint16_t) fetched + (uint16_t) getFlag(C);

    //Carry Flag is set if number exceeds 8 bit
    setFlag(C, temp>255);

    //Zero Flag is set if resultant is zero
    setFlag(Z, (temp & 0x00FF) == 0);

    //Negative Flag is set if msb bit is 1
    setFlag(N, (temp & 0x80));

    //For overflow flag: Since in 8 bits -128-127 can also be represented, we need to know
    //whether there has been a wrap around and hence overflow flag is used;
    setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

    //Store the result
    a = temp & 0x00FF;

    return 1;
}


uint8_t CPU6502::SBC(){
    fetch();

    //invert the least significant 8 bits
    uint16_t value = ((uint16_t) fetched) ^ 0x00FF;
    //If C = 1 (no borrow), B = 0.
    //If C = 0 (borrow occurred), B = 1.
    //Hence Formula = A - M - (1 - C) == A + ~M + C;
    temp = (uint16_t) a + (uint16_t) getFlag(C) + value;

    setFlag(C, temp & 0xFF00);
	setFlag(Z, ((temp & 0x00FF) == 0));
	setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	setFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}

uint8_t CPU6502::AND(){
    fetch();
    a = a & fetched;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 1;
}

uint8_t CPU6502::ASL(){
    fetch();
    temp = (uint16_t) fetched << 1;
    setFlag(C, (temp & 0xFF00) > 0);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x80);
    if(lookup.at(opcode).addrMode == &CPU6502::IMP){
        a = temp & 0x00FF;
    }else{
        write(addrAbs, temp & 0x00FF);
    }
    return 0;
}

//Branch if Carry flag clear
uint8_t CPU6502::BCC(){
    if(getFlag(C) == 0){
        cycles++;
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}

//Branch if carry set
uint8_t CPU6502::BCS(){
    if(getFlag(C) == 1){

        //Add a cycle for branching
        cycles++;

        //For Relative Addressing mode we stored the jump in addrRel in the 
        //addressing mode function
        addrAbs = pc + addrRel;

        //If branch to a new page then we need 2 cycles
        //Since we increased one cycle already, we increase one more here
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}	

//Branch if zero flag is set
uint8_t CPU6502::BEQ(){
    if(getFlag(Z) == 1){
        cycles++;   
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}	


uint8_t CPU6502::BIT(){
    fetch();
    temp = a & fetched;
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, fetched & (1 << 7));
    setFlag(V, fetched & (1 << 6));
    return 0;
}	

//Branch if negative flag is set
uint8_t CPU6502::BMI(){ 
    if(getFlag(N) == 1){
        cycles++;
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}

//Branch if zero flag is clear
uint8_t CPU6502::BNE(){
    if(getFlag(Z) == 0){
        cycles++;
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}	

//Branch if negative flag is clear
uint8_t CPU6502::BPL(){
    if(getFlag(N) == 0){
        cycles++;
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}	

uint8_t CPU6502::BRK(){
    pc++;
	
	setFlag(I, 1);
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	setFlag(B, 1);
	write(0x0100 + sp, pStatus);
	sp--;
	setFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}	

uint8_t CPU6502::BVC(){
    if(getFlag(V) == 0){
        cycles++;
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}


uint8_t CPU6502::BVS(){
    if(getFlag(V) == 1){
        cycles++;
        addrAbs = pc + addrRel;

        //If branch to a new page
        if((addrAbs & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addrAbs;
    }
    return 0;
}	

uint8_t CPU6502::CLC(){
    setFlag(C, false);
    return 0;
}	

uint8_t CPU6502::CLD(){
    setFlag(D, false);
    return 0;
}	

uint8_t CPU6502::CLI(){
    setFlag(I, false);
    return 0;
}


uint8_t CPU6502::CLV(){
    setFlag(V, false);
    return 0;
}	

uint8_t CPU6502::CMP(){
    fetch();
    temp = (uint16_t) a - (uint16_t) fetched;
    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, a >= fetched);
    return 1;
}	

uint8_t CPU6502::CPX(){
    fetch();
    temp = (uint16_t) x - (uint16_t) fetched;
    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, x >= fetched);
    return 0;
}	

uint8_t CPU6502::CPY(){
    fetch();
    temp = (uint16_t) y - (uint16_t) fetched;
    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, y >= fetched);
    return 0;
}


uint8_t CPU6502::DEC(){
    fetch();
    temp = fetched - 1;
    write(addrAbs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}	

uint8_t CPU6502::DEX(){
    x--;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}	

uint8_t CPU6502::DEY(){
    y--;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}	

uint8_t CPU6502::EOR(){
    fetch();
    a = a ^ fetched;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 1;
}


uint8_t CPU6502::INC(){
    fetch();
    temp = fetched + 1;
    write(addrAbs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}	

uint8_t CPU6502::INX(){
    x++;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}	

uint8_t CPU6502::INY(){
    y++;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}	

uint8_t CPU6502::JMP(){
    pc = addrAbs;
    return 0;
}


uint8_t CPU6502::JSR(){
    pc--;
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;
    pc = addrAbs;
    return 0;
}	

uint8_t CPU6502::LDA(){
    fetch();
    a = fetched;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 1;
}	

uint8_t CPU6502::LDX(){
    fetch();
    x = fetched;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 1;
}	

uint8_t CPU6502::LDY(){
    fetch();
    y = fetched;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 1;
}


uint8_t CPU6502::LSR(){
    fetch();
    setFlag(C, fetched & 0x0001);
    temp = fetched >> 1;
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    if(lookup.at(opcode).addrMode == &CPU6502::IMP){
        a = temp & 0x00FF;
    }else{
        write(addrAbs, temp & 0x00FF);
    }
    return 0;
}	

uint8_t CPU6502::NOP(){
    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
        }
        return 0;
}	

uint8_t CPU6502::ORA(){
    fetch();
    a = a | fetched;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 1;
}	

//Stack is located at page number 1 in the memory i.e. 0x0100 to 0x01FF -> 256 Bytes of Stack Memory
uint8_t CPU6502::PHA(){
    write(0x0100 + sp, a);
    sp--;
    return 0;
}


uint8_t CPU6502::PHP(){
    write(0x0100 + sp, pStatus | B | U);
    setFlag(B, 0);
    setFlag(U, 0);
    sp--;
    return 0;
}	

uint8_t CPU6502::PLA(){
    sp++;
    a = read(0x0100 + sp);
    setFlag(N, a & 0x80);
    setFlag(Z, a == 0x00);
    return 0;
}	

uint8_t CPU6502::PLP(){
    sp++;
    pStatus = read(0x0100 + sp);
    setFlag(U, 1);
    return 0;
}	

uint8_t CPU6502::ROL(){
    fetch();

    temp = (uint16_t) (fetched << 1) | getFlag(C);
    setFlag(C, temp & 0xFF00);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    if(lookup.at(opcode).addrMode == &CPU6502::IMP){
        a = temp & 0x00FF;
    }else{
        write(addrAbs, temp & 0x00FF);
    }
    return 0;

}


uint8_t CPU6502::ROR(){
	fetch();
	temp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
	setFlag(C, fetched & 0x01);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x0080);
	if (lookup.at(opcode).addrMode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addrAbs, temp & 0x00FF);
	return 0;
}	

uint8_t CPU6502::RTI(){
    sp++;
    pStatus = read(0x0100 + sp);
    pStatus &= ~B;
    pStatus &= ~U;

    sp++;
    pc = (uint16_t) read(sp + 0x0100);
    sp++;
    pc |= (uint16_t) read(sp + 0x0100) << 8;
    return 0;
}	

uint8_t CPU6502::RTS(){
    sp++;
    pc = (uint16_t)read(sp + 0x0100);
    sp++;
    pc |= (uint16_t)read(sp + 0x0100) << 8;

    pc++;
    return 0; 
}	

uint8_t CPU6502::SEC(){
    setFlag(C, true);
    return 0;
}	

uint8_t CPU6502::SED(){
    setFlag(D, true);
    return 0;
}	

uint8_t CPU6502::SEI(){
    setFlag(I, true);
    return 0;
}	

uint8_t CPU6502::STA(){
    write(addrAbs, a);
    return 0;
}


uint8_t CPU6502::STX(){
    write(addrAbs, x);
    return 0;
}	

uint8_t CPU6502::STY(){
    write(addrAbs, y);
    return 0;
}	

uint8_t CPU6502::TAX(){
    x = a;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}	

uint8_t CPU6502::TAY(){
    y = a;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}


uint8_t CPU6502::TSX(){
    x = sp;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}	

uint8_t CPU6502::TXA(){
    a = x;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 0;
}	

uint8_t CPU6502::TXS(){
    sp = x;
    return 0;
}	

uint8_t CPU6502::TYA(){
    a = y;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 0;
}


//Helper function to disassemble

std::map<uint16_t, std::string> CPU6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cpuRead(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup.at(opcode).addrMode == &CPU6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::IMM)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::ZP0)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;												
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::ZPX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::ZPY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::IZX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::IZY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::ABS)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::ABX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::ABY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::IND)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup.at(opcode).addrMode == &CPU6502::REL)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}
