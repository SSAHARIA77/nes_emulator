#include "APU2A03.h"

APU2A03::APU2A03(){

}

APU2A03::~APU2A03(){

}

uint8_t APU2A03::cpuRead(uint16_t addr){
    return 0x00;
}

void APU2A03::cpuWrite(uint16_t addr, uint8_t data){
    switch(addr){
        case 0x4000:
            switch ((data & 0xC0) >> 6) {
                case 0x00:
                    pulse1Seq.sequence = 0b01000000;
                    pulse1Osc.dutycycle = 0.125;
                    break;
                case 0x01:
                    pulse1Seq.sequence = 0b01100000;
                    pulse1Osc.dutycycle = 0.250;
                    break;
                case 0x02:
                    pulse1Seq.sequence = 0b01111000;
                    pulse1Osc.dutycycle = 0.500;
                    break;
                case 0x03:
                    pulse1Seq.sequence = 0b10011111;
                    pulse1Osc.dutycycle = 0.750;
                    break;
            }
            break;
        case 0x4001:
            break;
        case 0x4002:
            pulse1Seq.reload = (pulse1Seq.reload & 0xFF00) | data;
            break;
        case 0x4003:
            pulse1Seq.reload =
                (uint16_t)((data & 0x07)) << 8 | (pulse1Seq.reload & 0x00FF);
            pulse1Seq.timer = pulse1Seq.reload;
            break;
        case 0x4004:
            break;
        case 0x4005:
            break;
        case 0x4006:
            break;
        case 0x4007:
            break;
        case 0x4008:
            break;
        case 0x400C:
            break;
        case 0x400E:
            break;
        case 0x4015:
            pulse1Enable = data & 0x01;
            break;
        case 0x400F:
            break;
    }
}


void APU2A03::clock(){
    bool quarterFrameClock = false;
    bool halfFrameClock = false;
    globalTime += (0.3333333333 / 1789773);
    if(clockCounter % 6 == 0){
        frameClockCounter++;
        // 4-Step Sequence Mode
        if (frameClockCounter == 3729) {
            quarterFrameClock = true;
        }

        if (frameClockCounter == 7457) {
            quarterFrameClock = true;
            halfFrameClock = true;
        }

        if (frameClockCounter == 11186) {
            quarterFrameClock = true;
        }

        if (frameClockCounter == 14916) {
            quarterFrameClock = true;
            halfFrameClock = true;
            frameClockCounter = 0;
        }

        // Update functional units

        // Quater frame "beats" adjust the volume envelope
        if (quarterFrameClock) {

        }

        // Half frame "beats" adjust the note length and
        // frequency sweepers
        if (halfFrameClock) {

        }

        // pulse1Seq.clock(pulse1Enable, [](uint32_t &s){
        //     s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
        // });

        pulse1Osc.frequency = 1789773.0 / (16.0 * (double)(pulse1Seq.reload + 1));
        pulse1Sample = (double) pulse1Osc.sample(globalTime);
    }

    clockCounter++;
}

void APU2A03::reset(){

}

double APU2A03::GetOutputSample(){
    return pulse1Sample;
}
