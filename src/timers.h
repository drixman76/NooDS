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

#ifndef TIMERS_H
#define TIMERS_H

#include <cstdint>

class Interpreter;

class Timers
{
    public:
        Timers(Interpreter *cpu): cpu(cpu) {}

        void tick();
        void doubleTick();

        bool shouldTick() { return enabled; }

        uint8_t readTmCntL(unsigned int timer, unsigned int byte) { return tmCntL[timer] >> (byte * 8); }
        uint8_t readTmCntH(unsigned int timer)                    { return tmCntH[timer];               }

        void writeTmCntL(unsigned int timer, unsigned int byte, uint8_t value);
        void writeTmCntH(unsigned int timer, uint8_t value);

    private:
        uint16_t tmCntL[4] = {};
        uint8_t tmCntH[4] = {};

        uint16_t reloads[4] = {};
        uint16_t scalers[4] = {};
        uint8_t enabled = 0;

        Interpreter *cpu;
};

#endif // TIMERS_H
