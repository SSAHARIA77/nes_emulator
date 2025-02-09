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
                    uint8_t pixel = (tileLsb & 0x01) + (tileMsb & 0x01);
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
    if (bReadOnly) {
        // Reading from PPU registers can affect their contents
        // so this read only option is used for examining the
        // state of the PPU without changing its state. This is
        // really only used in debug mode.
        switch (addr) {
            case 0x0000:  // Control
                data = control.reg;
                break;
            case 0x0001:  // Mask
                data = mask.reg;
                break;
            case 0x0002:  // Status
                data = status.reg;
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
                break;
        }
    }else{
        switch (addr) {
            case 0x0000:  // Control
                break;
            case 0x0001:  // Mask
                break;
            case 0x0002:  // Status
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
                ppu_data_buffer = ppuRead(vramAddr.reg);

                // No delay for the palette range
                if (vramAddr.reg >= 0x3F00) data = ppu_data_buffer;
                vramAddr.reg += (control.increment_mode ? 32 : 1);
                break;
        }
    }
    
    return data;
}

void PPU2C02::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000:  // Control
            control.reg = data;
            tramAddr.nametable_x = control.nametable_x;
            tramAddr.nametable_y = control.nametable_y;
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
            if(address_latch == 0){
                fineX = data & 0x07;
                tramAddr.coarse_x = data >> 3;
                address_latch = 1;
            }
            else{
                tramAddr.fine_y= data & 0x07;
                tramAddr.coarse_y = data >> 3;
                address_latch = 0;
            }
            break;
        case 0x0006:  // PPU Address
            //Storing the low byte and high byte of the address
            if (address_latch == 0) {
                tramAddr.reg = uint16_t(((data & 0x3F) << 8) | (tramAddr.reg & 0x00FF));
                address_latch = 1;
            } else {
                tramAddr.reg = (tramAddr.reg & 0xFF00) | data;
                vramAddr = tramAddr;
                address_latch = 0;
            }
            break;

        case 0x0007:  // PPU Data
            ppuWrite(vramAddr.reg, data);
            vramAddr.reg += (control.increment_mode ? 32 : 1);
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
        addr &= 0x0FFF;
        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) 
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF) 
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF) 
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF) 
                data = tblName[1][addr & 0x03FF];

        } else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }

    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
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
        addr &= 0x0FFF;
        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;

        } else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;
        }

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
    auto IncrementScrollX = [&]() {
        // Note: pixel perfect scrolling horizontally is handled by the
        // data shifters. Here we are operating in the spatial domain of
        // tiles, 8x8 pixel blocks.

        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites) {
            // A single name table is 32x30 tiles. As we increment horizontally
            // we may cross into a neighbouring nametable, or wrap around to
            // a neighbouring nametable
            if (vramAddr.coarse_x == 31) {
                // Leaving nametable so wrap address round
                vramAddr.coarse_x = 0;
                // Flip target nametable bit
                vramAddr.nametable_x = ~vramAddr.nametable_x;
            } else {
                // Staying in current nametable, so just increment
                vramAddr.coarse_x++;
            }
        }
    };

    auto IncrementScrollY = [&]() {
        // Incrementing vertically is more complicated. The visible nametable
        // is 32x30 tiles, but in memory there is enough room for 32x32 tiles.
        // The bottom two rows of tiles are in fact not tiles at all, they
        // contain the "attribute" information for the entire table. This is
        // information that describes which palettes are used for different
        // regions of the nametable.

        // In addition, the NES doesnt scroll vertically in chunks of 8 pixels
        // i.e. the height of a tile, it can perform fine scrolling by using
        // the fine_y component of the register. This means an increment in Y
        // first adjusts the fine offset, but may need to adjust the whole
        // row offset, since fine_y is a value 0 to 7, and a row is 8 pixels
        // high

        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites) {
            // If possible, just increment the fine y offset
            if (vramAddr.fine_y < 7) {
                vramAddr.fine_y++;
            } else {
                // If we have gone beyond the height of a row, we need to
                // increment the row, potentially wrapping into neighbouring
                // vertical nametables. Dont forget however, the bottom two rows
                // do not contain tile information. The coarse y offset is used
                // to identify which row of the nametable we want, and the fine
                // y offset is the specific "scanline"

                // Reset fine y offset
                vramAddr.fine_y = 0;

                // Check if we need to swap vertical nametable targets
                if (vramAddr.coarse_y == 29) {
                    // We do, so reset coarse y offset
                    vramAddr.coarse_y = 0;
                    // And flip the target nametable bit
                    vramAddr.nametable_y = ~vramAddr.nametable_y;
                } else if (vramAddr.coarse_y == 31) {
                    // In case the pointer is in the attribute memory, we
                    // just wrap around the current nametable
                    vramAddr.coarse_y = 0;
                } else {
                    // None of the above boundary/wrapping conditions apply
                    // so just increment the coarse y offset
                    vramAddr.coarse_y++;
                }
            }
        }
    };

    auto TransferAddressX = [&]() {
        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites) {
            vramAddr.nametable_x = tramAddr.nametable_x;
            vramAddr.coarse_x = tramAddr.coarse_x;
        }
    };

    auto TransferAddressY = [&]() {
        // Ony if rendering is enabled
        if (mask.render_background || mask.render_sprites) {
            vramAddr.fine_y = tramAddr.fine_y;
            vramAddr.nametable_y = tramAddr.nametable_y;
            vramAddr.coarse_y = tramAddr.coarse_y;
        }
    };

    auto LoadBackgroundShifters = [&]() {
        // Each PPU update we calculate one pixel. These shifters shift 1 bit
        // along feeding the pixel compositor with the binary information it
        // needs. Its 16 bits wide, because the top 8 bits are the current 8
        // pixels being drawn and the bottom 8 bits are the next 8 pixels to be
        // drawn. Naturally this means the required bit is always the MSB of the
        // shifter. However, "fine x" scrolling plays a part in this too, whcih
        // is seen later, so in fact we can choose any one of the top 8 bits.
        bgShifterPatternLo =
            (bgShifterPatternLo & 0xFF00) | bgNextTileLsb;
        bgShifterPatternHi =
            (bgShifterPatternHi & 0xFF00) | bgNextTileMsb;

        // Attribute bits do not change per pixel, rather they change every 8
        // pixels but are synchronised with the pattern shifters for
        // convenience, so here we take the bottom 2 bits of the attribute word
        // which represent which palette is being used for the current 8 pixels
        // and the next 8 pixels, and "inflate" them to 8 bit words.
        bgShifterAttribLo = (bgShifterAttribLo & 0xFF00) |
                               ((bgNextTileAttribute & 0b01) ? 0xFF : 0x00);
        bgShifterAttribHi = (bgShifterAttribHi & 0xFF00) |
                               ((bgNextTileAttribute & 0b10) ? 0xFF : 0x00);
    };

    auto UpdateShifters = [&]() {
        if (mask.render_background) {
            // Shifting background tile pattern row
            bgShifterPatternLo <<= 1;
            bgShifterPatternHi <<= 1;

            // Shifting palette attributes by 1
            bgShifterAttribLo <<= 1;
            bgShifterAttribHi <<= 1;
        }
    };

    //Started rendering on visible screen
    if(scanline >= -1 &&  scanline < 240){

        if(cycle == 0 && scanline == 0){
            cycle = 1;  
        }

        if (scanline == -1 && cycle == 1) {
            status.vertical_blank = 0;
        }

        if((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)){
            UpdateShifters();
            switch((cycle - 1) % 8){
                case 0:
                    LoadBackgroundShifters();
                    bgNextTileId = ppuRead(0x2000 | (vramAddr.reg & 0x0FFF));
                    break;
                case 2: 
                    bgNextTileAttribute = ppuRead(0x23C0 | (vramAddr.nametable_y << 11)
                        | (vramAddr.nametable_x << 10)
                        | ((vramAddr.coarse_y >> 2 ) << 3)
                        | (vramAddr.coarse_x >> 2));
                    if(vramAddr.coarse_y & 0x02) bgNextTileAttribute >>= 4;
                    if(vramAddr.coarse_x & 0x02) bgNextTileAttribute >>= 2;
                    bgNextTileAttribute &= 0x03;
                    break;
                case 4:
                    bgNextTileLsb = ppuRead((control.pattern_background << 12)
                        + ((uint16_t) bgNextTileId << 4)
                        + (vramAddr.fine_y) + 0);
                    break;
                case 6:
                    bgNextTileMsb = ppuRead((control.pattern_background << 12) 
                    + ((uint16_t)bgNextTileId << 4) 
                    + (vramAddr.fine_y) + 8);
                    break;
                case 7:
                    IncrementScrollX();
                    break;
            }
        }

        if(cycle == 256){
            IncrementScrollY();
        }

        if(cycle == 257){
            LoadBackgroundShifters();
            TransferAddressX();
        }

        if(cycle == 338 || cycle == 340){
            bgNextTileId = ppuRead(0x2000 | (vramAddr.reg & 0x0FFF));
        }

        if(scanline == -1 && cycle >= 280 && cycle < 305){
            TransferAddressY();
        }

    }

    //Rendering on hidden screen -> CPU can now interact with ppu
    //Basically when cathode ray tube moves back to the top, it is the time cpu can make changes to the graphics
    //through ppu to update the screen in the next render cycle. This is done to avoid screen glitches and 
    //artifacts that might be caused if the cpu writes in middle of render period.
     if(scanline == 240){
        //post render scanline - Nothing happens!
     }

     if(scanline >= 241 && scanline < 261){
        if (scanline == 241 && cycle == 1) {
            status.vertical_blank = 1;
            if (control.enable_nmi) {
                nmi = true;
            }
        }
     }


    uint8_t bgPixel = 0x00;
    uint8_t bgPalette = 0x00;

    if (mask.render_background) {
        // Handle Pixel Selection by selecting the relevant bit
        // depending upon fine x scolling. This has the effect of
        // offsetting ALL background rendering by a set number
        // of pixels, permitting smooth scrolling
        uint16_t bit_mux = 0x8000 >> fineX;

        // Select Plane pixels by extracting from the shifter
        // at the required location.
        uint8_t p0_pixel = (bgShifterPatternLo & bit_mux) > 0;
        uint8_t p1_pixel = (bgShifterPatternHi & bit_mux) > 0;
        // Combine to form pixel index
        bgPixel = (p1_pixel << 1) | p0_pixel;

        // Get palette
        uint8_t bg_pal0 = (bgShifterAttribLo & bit_mux) > 0;
        uint8_t bg_pal1 = (bgShifterAttribHi & bit_mux) > 0;
        bgPalette = (bg_pal1 << 1) | bg_pal0;
    }

    // Fake some noise for now, selecti ng black or white pixel
    sprScreen->SetPixel(cycle - 1, scanline, GetColourFromPaletteRam(bgPalette, bgPixel));

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


void PPU2C02::reset(){
    fineX = 0x00;
    address_latch = 0x00;
    ppu_data_buffer = 0x00;
    scanline = 0;
    cycle = 0;
    bgNextTileId = 0x00;
    bgNextTileAttribute = 0x00;
    bgNextTileLsb = 0x00;
    bgNextTileMsb = 0x00;
    bgShifterPatternLo = 0x0000;
    bgShifterPatternHi = 0x0000;
    bgShifterAttribLo = 0x0000;
    bgShifterAttribHi = 0x0000;
    status.reg = 0x00;
    mask.reg = 0x00;
    control.reg = 0x00;
    vramAddr.reg = 0x0000;
    tramAddr.reg = 0x0000;
}