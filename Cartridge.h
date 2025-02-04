#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include "Mapper_000.h"

class Cartridge{
    public:
        Cartridge(const std::string &sFileName);
        ~Cartridge();

        enum MIRROR{
            HORIZONTAL,
            VERTICAL, 
            ONESCREEN_LO,
            ONESCREEN_HI,
        }mirror = HORIZONTAL;


    private:
        // PRG ROM holds the game's code and data for the CPU (the game logic,
        // instructions, level data, music, etc.).
        std::vector<uint8_t> vPRGMemory;

        // CHR ROM stores the graphical data used in the game (pattern memory),
        // such as sprites (characters, enemies, etc.) and
        // background tiles
        std::vector<uint8_t>vCHRMemory; 

        bool bImageValid = false;

        //To know which mapper we are using
        uint8_t nMapperID = 0;
        //To know how many banks of the respective memory are present
        uint8_t nPRGBanks = 0;
        uint8_t nCHRBanks = 0;

        //The memory that can be addressed by the nes in cartridge is limited but the game data might be much
        //larger than that. Hence Mapper is used which translates the memory address coming from ppu or cpu to
        //the correct memory address on the cartridge. The Larger memory is split into banks. The mapper is configured
        //accordingly.
        std::shared_ptr<Mapper> pMapper;

    public:
        bool ImageValid();


    public:


        //To Communicate with the main bus
        bool cpuRead(uint16_t addr, uint8_t &data);
        bool cpuWrite(uint16_t addr, uint8_t data);

        //To Communicate with the ppu bus
        bool ppuRead(uint16_t addr, uint8_t &data);
        bool ppuWrite(uint16_t addr, uint8_t data);
};