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

#ifndef GPU_3D_H
#define GPU_3D_H

#include <cstdint>
#include <queue>

#include "defines.h"

class Interpreter;

struct Matrix
{
    int data[4 * 4] =
    {
        1 << 12, 0 << 12, 0 << 12, 0 << 12,
        0 << 12, 1 << 12, 0 << 12, 0 << 12,
        0 << 12, 0 << 12, 1 << 12, 0 << 12,
        0 << 12, 0 << 12, 0 << 12, 1 << 12
    };
};

struct Vertex
{
    int x = 0, y = 0, z = 0, w = 1 << 12;
    uint16_t color = 0x7FFF;
};

struct Polygon
{
    unsigned int size = 0;
    Vertex *vertices = nullptr;
};

struct Entry
{
    uint8_t command = 0;
    uint32_t param = 0;
};

class Gpu3D
{
    public:
        Gpu3D(Interpreter *arm9);

        void runCycle();

        bool shouldRun() { return gxStat & BIT(27); }

        Polygon     *getPolygons()     { return polygonsOut;     }
        unsigned int getPolygonCount() { return polygonCountOut; }

        uint8_t readGxStat(unsigned int byte) { return gxStat >> (byte * 8); }

        void writeGxFifo(unsigned int byte, uint8_t value);
        void writeMtxMode(unsigned int byte, uint8_t value);
        void writeMtxPush(unsigned int byte, uint8_t value);
        void writeMtxPop(unsigned int byte, uint8_t value);
        void writeMtxStore(unsigned int byte, uint8_t value);
        void writeMtxRestore(unsigned int byte, uint8_t value);
        void writeMtxIdentity(unsigned int byte, uint8_t value);
        void writeMtxLoad44(unsigned int byte, uint8_t value);
        void writeMtxLoad43(unsigned int byte, uint8_t value);
        void writeMtxMult44(unsigned int byte, uint8_t value);
        void writeMtxMult43(unsigned int byte, uint8_t value);
        void writeMtxMult33(unsigned int byte, uint8_t value);
        void writeMtxScale(unsigned int byte, uint8_t value);
        void writeMtxTrans(unsigned int byte, uint8_t value);
        void writeColor(unsigned int byte, uint8_t value);
        void writeNormal(unsigned int byte, uint8_t value);
        void writeTexCoord(unsigned int byte, uint8_t value);
        void writeVtx16(unsigned int byte, uint8_t value);
        void writeVtx10(unsigned int byte, uint8_t value);
        void writeVtxXY(unsigned int byte, uint8_t value);
        void writeVtxXZ(unsigned int byte, uint8_t value);
        void writeVtxYZ(unsigned int byte, uint8_t value);
        void writeVtxDiff(unsigned int byte, uint8_t value);
        void writePolygonAttr(unsigned int byte, uint8_t value);
        void writeTexImageParam(unsigned int byte, uint8_t value);
        void writePlttBase(unsigned int byte, uint8_t value);
        void writeDifAmb(unsigned int byte, uint8_t value);
        void writeSpeEmi(unsigned int byte, uint8_t value);
        void writeLightVector(unsigned int byte, uint8_t value);
        void writeLightColor(unsigned int byte, uint8_t value);
        void writeShininess(unsigned int byte, uint8_t value);
        void writeBeginVtxs(unsigned int byte, uint8_t value);
        void writeEndVtxs(unsigned int byte, uint8_t value);
        void writeSwapBuffers(unsigned int byte, uint8_t value);
        void writeViewport(unsigned int byte, uint8_t value);
        void writeBoxTest(unsigned int byte, uint8_t value);
        void writePosTest(unsigned int byte, uint8_t value);
        void writeVecTest(unsigned int byte, uint8_t value);
        void writeGxStat(unsigned int byte, uint8_t value);

    private:
        unsigned int matrixMode = 0;
        Matrix temp;
        Matrix projection;
        Matrix coordStack[32];
        Matrix direcStack[32];

        Polygon polygons1[2048] = {}, polygons2[2048] = {};
        Polygon *polygonsIn = polygons1, *polygonsOut = polygons2;
        unsigned int polygonCountIn = 0, polygonCountOut = 0;

        Vertex vertices1[6144] = {}, vertices2[6144] = {};
        Vertex *verticesIn = vertices1, *verticesOut = vertices2;
        unsigned int vertexCountIn = 0, vertexCountOut = 0;

        std::queue<Entry> fifo, pipe;

        unsigned int paramCounts[0x100] = {};
        unsigned int paramCount = 0;

        uint32_t gxFifoCmds = 0;
        unsigned int gxFifoCount = 0;

        uint32_t gxFifo = 0;
        uint32_t mtxMode = 0;
        uint32_t mtxPush = 0;
        uint32_t mtxPop = 0;
        uint32_t mtxStore = 0;
        uint32_t mtxRestore = 0;
        uint32_t mtxIdentity = 0;
        uint32_t mtxLoad44 = 0;
        uint32_t mtxLoad43 = 0;
        uint32_t mtxMult44 = 0;
        uint32_t mtxMult43 = 0;
        uint32_t mtxMult33 = 0;
        uint32_t mtxScale = 0;
        uint32_t mtxTrans = 0;
        uint32_t color = 0;
        uint32_t normal = 0;
        uint32_t texCoord = 0;
        uint32_t vtx16 = 0;
        uint32_t vtx10 = 0;
        uint32_t vtxXY = 0;
        uint32_t vtxXZ = 0;
        uint32_t vtxYZ = 0;
        uint32_t vtxDiff = 0;
        uint32_t polygonAttr = 0;
        uint32_t texImageParam = 0;
        uint32_t plttBase = 0;
        uint32_t difAmb = 0;
        uint32_t speEmi = 0;
        uint32_t lightVector = 0;
        uint32_t lightColor = 0;
        uint32_t shininess = 0;
        uint32_t beginVtxs = 0;
        uint32_t endVtxs = 0;
        uint32_t swapBuffers = 0;
        uint32_t viewport = 0;
        uint32_t boxTest = 0;
        uint32_t posTest = 0;
        uint32_t vecTest = 0;
        uint32_t gxStat = 0x04000000;

        Interpreter *arm9;

        Matrix multiply(Matrix *mtx1, Matrix *mtx2);
        Vertex multiply(Vertex *vtx, Matrix *mtx);

        void mtxModeCmd(uint32_t param);
        void mtxPushCmd();
        void mtxPopCmd(uint32_t param);
        void mtxStoreCmd(uint32_t param);
        void mtxRestoreCmd(uint32_t param);
        void mtxIdentityCmd();
        void mtxLoad44Cmd(uint32_t param);
        void mtxLoad43Cmd(uint32_t param);
        void mtxMult44Cmd(uint32_t param);
        void mtxMult43Cmd(uint32_t param);
        void mtxMult33Cmd(uint32_t param);
        void mtxScaleCmd(uint32_t param);
        void mtxTransCmd(uint32_t param);
        void colorCmd(uint32_t param);
        void vtx16Cmd(uint32_t param);
        void beginVtxsCmd(uint32_t param);
        void swapBuffersCmd(uint32_t param);

        void addEntry(Entry entry);
};

#endif // GPU_3D_H
