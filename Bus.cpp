#include "Bus.h"

Bus::Bus(){
    //Connect to cpu
    cpu.connectBus(this);
}

Bus::~Bus(){

}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly){
    uint8_t data = 0x00;
    if(cart->cpuRead(addr, data)){
        //Cartridge Address Range
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF){
        //System Ram address Range mirrored every 2048
        //Binary AND with a number 2^n - 1 effectively gives the remainder with 2^n
        data = cpuRam[addr & 0x07FF];
    } 

    else if(addr >= 0x2000 && addr <= 0x3FFF){
        //PPU Address Range mirrored every 8
        //Basically CPU only requires read and write to ppu at 8 locations from 0x2000 to 0x2007
        //These 8 bytes refers to the 8 registers of ppu including control, mask, status, scroll, etc.
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    }

    else if(addr >= 0x4016 && addr <= 0x4017){
        data = (controllerState[addr & 0x0001] & 0x80) > 0;
        controllerState[addr & 0x0001] <<= 1;
    }

    return data;
}


void Bus::cpuWrite(uint16_t addr, uint8_t data){
    if(cart->cpuWrite(addr, data)){
        //Cartridge Address Range
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF){
        //System Ram address Range mirrored every 2048
        cpuRam[addr & 0x07FF] = data;
        
    } 
    else if(addr >= 0x2000 && addr <= 0x3FFF){
        //PPU Address Range mirrored every 8
       ppu.cpuWrite(addr & 0x0007, data);
    }
    else if((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017){
        apu.cpuWrite(addr, data);
    }


    else if(addr == 0x4014){
        dmaPage = data;
        dmaAddr = 0x00;
        dmaTransfer = true;
    }
	else if (addr >= 0x4016 && addr <= 0x4017){
		controllerState[addr & 0x0001] = controller[addr & 0x0001]; 
	}
	
}

void Bus::reset(){
    cart->reset();
    cpu.reset();
    ppu.reset();
    nSystemClockCounter = 0;
    dmaPage = 0x00;
    dmaAddr = 0x00;
    dmaData = 0x00;
    dmaDummy = true;
    dmaTransfer = false;
}

bool Bus::clock(){
    ppu.clock();

    apu.clock();

	// The CPU runs 3 times slower than the PPU so we only call its
	// clock() function every 3 times this function is called. We
	// have a global counter to keep track of this.
    if(nSystemClockCounter % 3 == 0 ){
        if(dmaTransfer){
            if(dmaDummy){
                //Wait for the correct clock cycle for synchronization
                if(nSystemClockCounter % 2 == 1){
                    dmaDummy = false;
                }
            }else{
                if(nSystemClockCounter % 2 == 0){
                    //Read in Even cycle
                    dmaData = cpuRead(dmaPage << 8 | dmaAddr);
                }else{
                    //Write in odd cycle
                    ppu.pOAM[dmaAddr] = dmaData;
                    dmaAddr++;

                    if(dmaAddr == 0x00){
                        dmaTransfer = false;
                        dmaDummy = true;
                    }
                }
            }
        }else{
            cpu.clock();
        }
    }

    //Synchronizing with Audio
    bool audioSampleReady = false;
    audioTime += audioTimePerNESClock;
    if(audioTime >= audioTimePerSystemSample){
        audioTime -= audioTimePerSystemSample;
        audioSample = apu.GetOutputSample();
        audioSampleReady = true;
    }


    if(ppu.nmi){
        ppu.nmi = false;
        cpu.nonMaskableIrq();
    }
    nSystemClockCounter++;

    return audioSampleReady;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge){
    //Connects cartridge to both main bus and ppu bus
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}


void Bus::SetSampleFrequency(uint32_t sampleRate){
    audioTimePerSystemSample = 1.0/(double) sampleRate;
    audioTimePerNESClock = 1.0/5369318.0;
}