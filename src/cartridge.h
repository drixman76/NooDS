/*
    Copyright 2019 Hydr8gon

    This file is part of NooDS.

    NooDS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NooDS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NooDS. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>

#include "defines.h"

class Interpreter;
class Memory;

class Cartridge
{
    public:
        Cartridge(Interpreter *cpu, Memory *memory): cpu(cpu), memory(memory) {}

        void setRom(uint8_t *rom, unsigned int romSize, uint8_t *save, unsigned int saveSize);

        bool transferReady() { return romCtrl & BIT(23); }

        uint8_t readAuxSpiCnt(unsigned int byte) { return auxSpiCnt >> (byte * 8); }
        uint8_t readAuxSpiData()                 { return auxSpiData;              }
        uint8_t readRomCtrl(unsigned int byte)   { return romCtrl   >> (byte * 8); }
        uint8_t readRomCmdOut(unsigned int byte) { return romCmdOut >> (byte * 8); }
        uint8_t readRomDataIn(unsigned int byte);

        void writeAuxSpiCnt(unsigned int byte, uint8_t value);
        void writeAuxSpiData(uint8_t value);
        void writeRomCtrl(unsigned int byte, uint8_t value);
        void writeRomCmdOut(unsigned int byte, uint8_t value);

    private:
        uint64_t command = 0;
        unsigned int blockSize = 0, readCount = 0;
        bool encrypted = false;

        uint32_t encTable[0x412] = {};
        uint32_t encCode[3] = {};

        uint8_t auxCommand = 0;
        uint32_t auxAddress = 0;
        unsigned int auxWriteCount = 0;

        uint16_t auxSpiCnt = 0;
        uint8_t auxSpiData = 0;
        uint32_t romCtrl = 0;
        uint64_t romCmdOut = 0;
        uint32_t romDataIn = 0;

        uint8_t *rom = nullptr, *save = nullptr;
        unsigned int romSize = 0, saveSize = 0;

        Interpreter *cpu;
        Memory *memory;

        uint64_t encrypt64(uint64_t value);
        uint64_t decrypt64(uint64_t value);
        void initKeycode(unsigned int level);
        void applyKeycode();
};

#endif // CARTRIDGE_H
