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

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

class Cartridge;
class Cp15;
class Dma;
class Gpu;
class Gpu2D;
class Gpu3D;
class Input;
class Interpreter;
class Ipc;
class Math;
class Rtc;
class Spi;
class Timers;

class Memory
{
    public:
        Memory(Cartridge *cart9, Cartridge *cart7, Cp15 *cp15, Dma *dma9, Dma *dma7, Gpu *gpu, Gpu2D *engineA,
               Gpu2D *engineB, Gpu3D *gpu3D, Input *input, Interpreter *arm9, Interpreter *arm7,
               Ipc *ipc, Math *math, Rtc *rtc, Spi *spi, Timers *timers9, Timers *timers7);

        template <typename T> T read(bool arm9, uint32_t address);
        template <typename T> void write(bool arm9, uint32_t address, T value);

        uint8_t *getMappedVram(uint32_t address);
        uint8_t *getVramBlock(unsigned int block);
        uint8_t *getPalette() { return palette; }
        uint8_t *getOam() { return oam; }

    private:
        uint8_t ram[0x400000]    = {}; //  4MB main RAM
        uint8_t wram[0x8000]     = {}; // 32KB shared WRAM
        uint8_t instrTcm[0x8000] = {}; // 32KB instruction TCM
        uint8_t dataTcm[0x4000]  = {}; // 16KB data TCM
        uint8_t bios9[0x8000]    = {}; // 32KB ARM9 BIOS
        uint8_t bios7[0x4000]    = {}; // 16KB ARM7 BIOS
        uint8_t wram7[0x10000]   = {}; // 64KB ARM7 WRAM

        uint8_t palette[0x800] = {}; //   2KB palette
        uint8_t vramA[0x20000] = {}; // 128KB VRAM block A
        uint8_t vramB[0x20000] = {}; // 128KB VRAM block B
        uint8_t vramC[0x20000] = {}; // 128KB VRAM block C
        uint8_t vramD[0x20000] = {}; // 128KB VRAM block D
        uint8_t vramE[0x10000] = {}; //  64KB VRAM block E
        uint8_t vramF[0x4000]  = {}; //  16KB VRAM block F
        uint8_t vramG[0x4000]  = {}; //  16KB VRAM block G
        uint8_t vramH[0x8000]  = {}; //  32KB VRAM block H
        uint8_t vramI[0x4000]  = {}; //  16KB VRAM block I
        uint8_t oam[0x800]     = {}; //   2KB OAM

        uint8_t vramStat = 0, wramStat = 0;
        uint32_t vramBases[9] = {};

        uint32_t dmaFill[4] = {};
        uint8_t haltCnt = 0;
        uint16_t soundBias = 0;

        Cartridge *cart9, *cart7;
        Cp15 *cp15;
        Dma *dma9, *dma7;
        Gpu *gpu;
        Gpu2D *engineA, *engineB;
        Gpu3D *gpu3D;
        Input *input;
        Interpreter *arm9, *arm7;
        Ipc *ipc;
        Math *math;
        Rtc *rtc;
        Spi *spi;
        Timers *timers9, *timers7;

        template <typename T> T ioRead9(uint32_t address);
        template <typename T> T ioRead7(uint32_t address);
        template <typename T> void ioWrite9(uint32_t address, T value);
        template <typename T> void ioWrite7(uint32_t address, T value);

        uint8_t readWramStat()                                       { return wramStat;                       }
        uint8_t readDmaFill(unsigned int channel, unsigned int byte) { return dmaFill[channel] >> (byte * 8); }
        uint8_t readHaltCnt()                                        { return haltCnt;                        }
        uint8_t readSoundBias(unsigned int byte)                     { return soundBias        >> (byte * 8); }

        void writeVramCntA(uint8_t value);
        void writeVramCntB(uint8_t value);
        void writeVramCntC(uint8_t value);
        void writeVramCntD(uint8_t value);
        void writeVramCntE(uint8_t value);
        void writeVramCntF(uint8_t value);
        void writeVramCntG(uint8_t value);
        void writeWramCnt(uint8_t value);
        void writeVramCntH(uint8_t value);
        void writeVramCntI(uint8_t value);
        void writeDmaFill(unsigned int channel, unsigned int byte, uint8_t value);
        void writeHaltCnt(uint8_t value);
        void writeSoundBias(unsigned int byte, uint8_t value);
};

#endif // MEMORY_H
