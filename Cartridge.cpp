#include "Cartridge.h"

Cartridge::Cartridge(const std::string &sFileName) {
    // iNES Format Header
    // The nes files follow some standards and we are building according to it.
    // We are using the iNES format. Check here-
    // https://www.nesdev.org/wiki/INES --> Check here for the format
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bImageValid = false;

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if (ifs.is_open()) {
        // Read file header
        ifs.read((char *)&header, sizeof(sHeader));

        // If a "trainer" exists we just need to read past
        // it before we get to the good stuff
        // Used for some training, not for our use.
        if (header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);

        // Determine Mapper ID using the header we read
        // All details are given in the file format on the website
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        // "Discover" File Format
        // There are 3 types of iNES files
        // For now we are using only type 1 but we have left placeholders for
        // the other types for later.
        uint8_t nFileType = 1;

        if (nFileType == 0) {
        }

        if (nFileType == 1) {
            // Read the CHR and PRG data

            nPRGBanks = header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char *)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = header.chr_rom_chunks;
            vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char *)vCHRMemory.data(), vCHRMemory.size());
        }

        if (nFileType == 2) {
        }

        // Load appropriate mapper
        switch (nMapperID) {
            case 0:
                pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
                break;
        }

        bImageValid = true;
        ifs.close();
    }
}

Cartridge::~Cartridge() {}

bool Cartridge::ImageValid() { return bImageValid; }

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr)) {
        data = vPRGMemory[mapped_addr];
        return true;
    }

    return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr)) {
        vPRGMemory[mapped_addr] = data;
        return true;
    }

    return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = vCHRMemory[mapped_addr];
        return true;
    }

    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        vCHRMemory[mapped_addr] = data;
        return true;
    }

    return false;
}