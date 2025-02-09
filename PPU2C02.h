#pragma once

#include <cstdint>
#include <memory>

#include "Cartridge.h"
#include "olcPixelGameEngine.h"

class PPU2C02 {
   public:
    PPU2C02();
    ~PPU2C02();

   private:
    // Pattern Memory exists on the cartridge on normal NES systems
    // Wont even be used for the existing roms

    uint8_t tblPattern[2][4096];

    // PPU has 2kB of name table. But full name table is 1kB but NES has
    // capability to store 2 name tables

    // First 4 palettes are generally used for background palettes and last 4
    // for foreground palettes.
    uint8_t tblPalette[32];
    public:
        uint8_t tblName[2][1024];
    private:
        std::shared_ptr<Cartridge> cart;

   private:
    // To store all the colors that the NES can display
    olc::Pixel palScreen[0x40];
    // In Video
    // olc::Sprite sprScreen = olc::Sprite(256, 240);
    // olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256,
    // 240) }; olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128),
    // olc::Sprite(128, 128) };

    // Changed To for API breaking subsequent PGE Update

    // To represent the complete screen
    olc::Sprite* sprScreen;

    // To represent both the name tables
    olc::Sprite* sprNameTable[2];

    // To represent both the pattern table

    // Two 4kB sections, each section is split into 16*16 grid of
    // sprites/tile. Each tile is 8*8 pixel. Each pixel is 2 bits. So 4
    // colors per pixel. So each pixel can take a value of 0,1,2,3. Pixel
    // Value 0 is used as transparent. These pixel values with combination
    // with palettes can generate
    // the required colors.
    // These tiles are stored as two bit planes LSB plane and MSB plane. The
    // value of the pixel is the sum from these planes.

    // 1 Tile = 64 * 2 = 128 bits = 16 bytes
    // First 8 bits of tile represents the LSB plane followed by 8 bytes of
    // MSB Plane 16 * 16 tiles = 16 * 16 * 128 = 2^15 bits =  2^12 bytes =
    // 4kB

    // So both section is 128 pixel wide and long
    // Generally one section contains the sprites and the second section
    // contains the background sprites
    olc::Sprite* sprPatternTable[2];

   public:
    // Debugging Utilities
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t i);
    olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);
    olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
    bool frame_complete = false;

   private:
    union {
        struct {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };

        uint8_t reg;
    } status;

    union {
        struct {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;

    union PPUCTRL {
        struct {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1;  // unused
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;

    union loopy_register {
        // Credit to Loopy for working this out :D
        struct {
            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_x : 1;
            uint16_t nametable_y : 1;
            uint16_t fine_y : 3;
            uint16_t unused : 1;
        };

        uint16_t reg = 0x0000;
    };

    loopy_register vramAddr;
    loopy_register tramAddr;

    uint8_t fineX = 0x00;

    uint16_t bgShifterPatternLo = 0x0000;
    uint16_t bgShifterPatternHi = 0x0000;
    uint16_t bgShifterAttribLo = 0x0000;
    uint16_t bgShifterAttribHi = 0x0000;

    uint8_t bgNextTileId = 0x00;
    uint8_t bgNextTileAttribute = 0x00;
    uint8_t bgNextTileLsb = 0x00;
    uint8_t bgNextTileMsb = 0x00;

    uint8_t address_latch =
        0x00;  // to know if we are using low byte or high byte when reading/writing to ppu
    uint8_t ppu_data_buffer = 0x00;  // when we read from ppu it is delayed by
                                     // one cyle so we need to buffer the byte

    // Scanline represents which row on the screen
    int16_t scanline = 0;
    // Cycle represents which columns on the screen
    int16_t cycle = 0;

   public:
    // To Communicate with the main bus
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // To Communicate with the ppu bus
    uint8_t ppuRead(uint16_t addr, bool bReadOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
    void reset();
    bool nmi = false;


};