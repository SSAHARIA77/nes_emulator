#include "PPU2C02.h"

PPU2C02::PPU2C02() {
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);

    sprScreen = new olc::Sprite(256, 240);
    sprNameTable[0] = new olc::Sprite(256, 240);
    sprNameTable[1] = new olc::Sprite(256, 240);
    sprPatternTable[0] = new olc::Sprite(128, 128);
    sprPatternTable[1] = new olc::Sprite(128, 128);
}

PPU2C02::~PPU2C02() {
    delete sprScreen;
    delete sprNameTable[0];
    delete sprNameTable[1];
    delete sprPatternTable[0];
    delete sprPatternTable[1];
}

olc::Sprite &PPU2C02::GetScreen() { return *sprScreen; }

olc::Sprite &PPU2C02::GetNameTable(uint8_t i) { return *sprNameTable[i]; }

//i represents which pattern table is being used
olc::Sprite &PPU2C02::GetPatternTable(uint8_t i, uint8_t palette) {
    for (uint16_t tileY{}; tileY < 16; tileY++) {
        for (uint16_t tileX{}; tileX < 16; tileX++) {
            //To reach the actual tile memory in 1D coordinates
            //Note that width of one row of tiles is 16 tiles * 16 bytes = 256 bytes.
            uint16_t offset = tileY * 256 + tileX * 16; 

            //For each tile there are 8 rows and 8 columns i.e. 64pixels
            for (uint16_t row{}; row < 8; row++) {
                uint8_t tileLsb = ppuRead(i * 0x1000 + offset + row + 0x0000);
                uint8_t tileMsb = ppuRead(i * 0x1000 + offset + row + 0x0008);

                for (uint16_t col{}; col < 8; col++) {
                    //Adding the value of LSB plane and MSB plane to get a value b/w 0 and 3 for the pixel
                    uint8_t pixel = (tileLsb & 0x01) + ((tileMsb & 0x01) << 1);
                    tileLsb >>= 1;
                    tileMsb >>= 1;

                    sprPatternTable[i]->SetPixel(
                        tileX * 8 + (7 - col), tileY * 8 + row,
                        GetColourFromPaletteRam(palette, pixel));
                }
            }
        }
    }
    return *sprPatternTable[i];
}

//Get the color from the palette id and pixel value (0,1,2,3)
olc::Pixel &PPU2C02::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel) {
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}
 
uint8_t PPU2C02::cpuRead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;

    switch (addr) {
        case 0x0000:  // Control
            break;
        case 0x0001:  // Mask
            break;
        case 0x0002:  // Status
            status.vertical_blank = 1;
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
            status.vertical_blank = 0;
            address_latch = 0;
            break;
        case 0x0003:  // OAM Address
            break;
        case 0x0004:  // OAM Data
            break;
        case 0x0005:  // Scroll
            break;
        case 0x0006:  // PPU Address
            break;
        case 0x0007:  // PPU Data
            data = ppu_data_buffer;
            ppu_data_buffer = ppuRead(ppu_address);

            // No delay for the palette range
            if (ppu_address > 0x3F00) data = ppu_data_buffer;
            ppu_address++;
            break;
    }

    return data;
}

void PPU2C02::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000:  // Control
            control.reg = data;
            break;
        case 0x0001:  // Mask
            mask.reg = data;
            break;
        case 0x0002:  // Status
            break;
        case 0x0003:  // OAM Address
            break;
        case 0x0004:  // OAM Data
            break;
        case 0x0005:  // Scroll
            break;
        case 0x0006:  // PPU Address
            //Storing the low byte and high byte of the address
            if (address_latch == 0) {
                ppu_address = (ppu_address & 0x00FF) | (data << 8);
                address_latch = 1;
            } else {
                ppu_address = (ppu_address & 0xFF00) | data;
                address_latch = 0;
            }
            break;

        case 0x0007:  // PPU Data
            ppuWrite(ppu_address, data);
            ppu_address++;
            break;
    }
}

uint8_t PPU2C02::ppuRead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    if (cart->ppuRead(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];

    } else if (addr >= 0x2000 && addr <= 0x3EFF) {
        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) {
                data = tblName[0][addr & 0x03FF];
            }

            if (addr >= 0x0400 && addr <= 0x07FF) {
                data = tblName[1][addr & 0x03FF];
            }

            if (addr >= 0x0800 && addr <= 0x0BFF) {
                data = tblName[0][addr & 0x03FF];
            }

            if (addr >= 0x0C00 && addr <= 0x0FFF) {
                data = tblName[1][addr & 0x03FF];
            }

        } else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
        }

    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        data = tblPalette[addr];
    }

    return data;
}

void PPU2C02::ppuWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;

    if (cart->ppuWrite(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) {

        //Pattern memory is a rom but just in case
        tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;

    } else if (addr >= 0x2000 && addr <= 0x3EFF) {
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        tblPalette[addr] = data;
    }
}

void PPU2C02::clock() {

    //Started rendering on visible screen
    if (scanline == -1 && cycle == 1) {
        status.vertical_blank = 0;
    }

    //Rendering on hidden screen -> CPU can now interact with ppu
    //Basically when cathode ray tube moves back to the top, it is the time cpu can make changes to the graphics
    //through ppu to update the screen in the next render cycle. This is done to avoid screen glitches and 
    //artifacts that might be caused if the cpu writes in middle of render period.
    
    if (scanline == 241 && cycle == 1) {
        status.vertical_blank = 1;
        if (control.enable_nmi) {
            nmi = true;
        }
    }

    // Fake some noise for now, selecting black or white pixel
    sprScreen->SetPixel(cycle - 1, scanline,
                        palScreen[(rand() % 2) ? 0x3F : 0x30]);

    // Advance renderer - it never stops, it's relentless
    cycle++;

    //Basically we scrolling through the screen and when all rows and columns are complete, the frame is complete
    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }
}

void PPU2C02::connectCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
}
