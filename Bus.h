#pragma once
#include "CPU6502.h"
#include <array>
#include <cstdint>
#include "PPU2C02.h"
#include "Cartridge.h"
#include "APU2A03.h"

// Ram array?
/*
g++ olcNes_Sounds1.cpp Bus.cpp CPU6502.cpp PPU2C02.cpp Cartridge.cpp Mapper_000.cpp Mapper.cpp -o marioFG -lopengl32 -lglu32 -lgdi32 -luser32 -lgdiplus -lshlwapi -ldwmapi
*/

class Bus{

    public:
        Bus();
        ~Bus();
    
    //Connect bus to cpu
    public:
        //Devices Connected to the Bus
        CPU6502 cpu;
        PPU2C02 ppu;
        APU2A03 apu;
        std::shared_ptr<Cartridge> cart;

        double audioSample = 0.0;
        void SetSampleFrequency(uint32_t sampleRate);
        
        
        //NES has effectively 2kB of ram but its address range is 
        //$0000 -  $1FFF which is 8kB memory.
        //Concept of mirrorring is used here and the first 2kB of memory is the actual hardware memory. The
        //rest three 2kB chunks are effectively mapped to the original 2kB section.
        uint8_t cpuRam[2048]; 
        uint8_t controller[2];
        
        //Functionalities
        
        //Read and Write from and to CPU
        void cpuWrite(uint16_t addr, uint8_t data);
        uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
        
        
        //System Interface
        //Our bus basically represents the complete NES system.
        void reset();
        bool clock();
        void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
        
    private:
        double audioTimePerSystemSample = 0.0;  
        double audioTimePerNESClock = 0.0;
        double audioTime = 0.0;


    private:
        uint32_t nSystemClockCounter = 0;
        uint8_t controllerState[2];

        uint8_t dmaPage = 0x00;
        uint8_t dmaAddr = 0x00;
        uint8_t dmaData = 0x00;

        bool dmaTransfer = false;
        bool dmaDummy = true;
};