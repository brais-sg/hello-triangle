/**
 * @file main.cpp
 * @author Brais Solla G.
 * @brief hello-triangle on Nintendo DS (libnds)
 * @version 0.1
 * @date 2022-05-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <nds.h>

typedef volatile uint8_t  vu8_t;
typedef volatile uint16_t vu16_t;
typedef volatile uint32_t vu32_t;

typedef int16_t vfixed16_t;

typedef uint32_t drawlist_entry_t;

#ifndef _BV
#define _BV(x) ((1) << (x))
#endif

#define FLOAT2VFIXED16(n) ((vfixed16_t)((n) * (1 << 12)))

// http://problemkaputt.de/gbatek.htm#ds3diomap
#define DISP3DCNT   *((vu16_t*) 0x4000060)
#define CLEAR_COLOR *((vu32_t*) 0x4000350)
#define CLEAR_DEPTH *((vu16_t*) 0x4000354)

#define GXFIFO_ADDR 0x4000400
#define GXFIFO      *((vu32_t*) GXFIFO_ADDR)

#define GXSTAT      *((vu32_t*) 0x4000600)

#define CLIPMTX_R   ((vu32_t*)  0x4000640)

// DMA definitions (http://problemkaputt.de/gbatek.htm#dsdmatransfers)
#define DMACH0SRC *((vu32_t*) 0x40000B0)
#define DMACH0DST *((vu32_t*) 0x40000B4)
#define DMACH0CR  *((vu32_t*) 0x40000B8)

#define DMA_TRIGGER_GCFIFO   (7 << 27)


// Geometry Engine commands
#define CMD_UNPACKED(x) (((x) & 0xff))
#define CMD_ARG(x)      ((x & 0xffffffff))


#define CMD_NOP      		CMD_UNPACKED(0x00)
#define CMD_MTXMODE  		CMD_UNPACKED(0X10)
#define CMD_MTXPUSH  		CMD_UNPACKED(0x11)
#define CMD_MTXPOP   		CMD_UNPACKED(0x12)
#define CMD_MTXSTORE 		CMD_UNPACKED(0x13)
#define CMD_MTXIDENTITY		CMD_UNPACKED(0x15)
#define CMD_MTXLOAD4X4      CMD_UNPACKED(0x16)

#define CMD_VTXCOLOR        CMD_UNPACKED(0x20)
#define CMD_VTX16           CMD_UNPACKED(0x23)
#define CMD_VTX10           CMD_UNPACKED(0x24)
#define CMD_POLYATTR        CMD_UNPACKED(0x29)

#define CMD_BEGINVTX        CMD_UNPACKED(0x40)
#define CMD_ENDVTX          CMD_UNPACKED(0x41)

#define CMD_SWPBUFFERS      CMD_UNPACKED(0x50)

#define CMD_VIEWPORT        CMD_UNPACKED(0x60)
#define CMD_VIEWPORT_ARGS(x,y,w,h)  ((x & 0xff) | ((y & 0xff) << 8) | ((w & 0xff) << 16) | ((h & 0xff) << 24))

#define VTX16_XYARGS(x,y)   (((y & 0xffff) << 16) | (x & 0xffff))
#define VTX16_ZARGS(z)      (z & 0xffff) 

#define CMD_MTXMODE_PROJECTION 0
#define CMD_MTXMODE_POSITION   1


drawlist_entry_t drawList[1024];
uint32_t drawListElements = 0;

void addEntry(drawlist_entry_t entry){
	drawlist_entry_t* current = (drawlist_entry_t*) drawList + drawListElements;

	if(drawListElements < 1024){
		*current = entry;
		drawListElements++;
	}
}

void addVertexEntry(int32_t entry){
	int32_t* current = (int32_t*) drawList + drawListElements;

	if(drawListElements < 1024){
		*current = entry;
		drawListElements++;
	}
}

void cpuSendDrawlist(){
	for(uint32_t i = 0; i < drawListElements; i++){
		GXFIFO = drawList[i];
	}
}

void dmaSendDrawlist(){
	// Lets GO DMA!
	while(DMACH0CR & DMA_BUSY);

	DMACH0SRC = (uint32_t) drawList;
	DMACH0DST = GXFIFO_ADDR;
	DMACH0CR  = DMA_ENABLE | DMA_32_BIT | DMA_DST_FIX | DMA_TRIGGER_GCFIFO | drawListElements;
}

void clearDrawlist(){
	drawListElements = 0;
}


int main() {	
	videoSetMode(MODE_0_3D);

	powerOn(POWER_3D_CORE | POWER_MATRIX);



	// TODO: Clear matrix stack
	// TODO: This, without API
	// glResetMatrixStack();
	// glFlush(0);

	// glClearColor(0,0,0,31);
	// glClearPolyID(0);

	// glClearDepth(GL_MAX_DEPTH);

	// GFX_TEX_FORMAT = 0;
	// GFX_POLY_FORMAT = 0;

	// Generate drawList
	clearDrawlist();

	// Reset projection matrix
	addEntry(CMD_MTXMODE);
	addEntry(CMD_MTXMODE_PROJECTION);
	addEntry(CMD_MTXIDENTITY);

	// Reset modelview matrix
	addEntry(CMD_MTXMODE);
	addEntry(CMD_MTXMODE_POSITION);
	addEntry(CMD_MTXIDENTITY);

	addEntry(CMD_VIEWPORT);
	addEntry((uint32_t) CMD_VIEWPORT_ARGS(0, 0, 255, 191));

	// Draw the triangle
	addEntry(CMD_BEGINVTX);
	addEntry(0);

	// Enable back surface drawing, front surface drawing (No culling), and solid alpha
	addEntry(CMD_POLYATTR);
	addEntry(_BV(6) | _BV(7) | (31 << 16));

	// First vertex
	addEntry(CMD_VTXCOLOR);
	addEntry(31); // Red

	addEntry(CMD_VTX16);
	addVertexEntry(VTX16_XYARGS(FLOAT2VFIXED16(0.f),FLOAT2VFIXED16(.8f)));
	addVertexEntry(VTX16_ZARGS(FLOAT2VFIXED16(0.f)));

	// Second vertex
	addEntry(CMD_VTXCOLOR);
	addEntry(31 << 5); // Green

	addEntry(CMD_VTX16);
	addVertexEntry(VTX16_XYARGS(FLOAT2VFIXED16(-.8f),FLOAT2VFIXED16(-.8f)));
	addVertexEntry(VTX16_ZARGS(FLOAT2VFIXED16(0.f)));

	// Third vertex
	addEntry(CMD_VTXCOLOR);
	addEntry(31 << 10); // Blue

	addEntry(CMD_VTX16);
	addVertexEntry(VTX16_XYARGS(FLOAT2VFIXED16(.8f),FLOAT2VFIXED16(-.8f)));
	addVertexEntry(VTX16_ZARGS(FLOAT2VFIXED16(0.f)));

	// End the drawlist
	// addEntry(CMD_ENDVTX);
	addEntry(CMD_SWPBUFFERS);
	addEntry(0);

	// Needed for DMA
	DC_FlushRange(drawList, drawListElements * sizeof(uint32_t));

	// Disable texturing, shading, fog...
	DISP3DCNT = 0;


	#define USE_DMA 1

	while(1) {
		scanKeys();

		u16 keys = keysDown();
		if(keys & KEY_START) break;

		#ifdef USE_DMA
		dmaSendDrawlist();
		#else
		cpuSendDrawlist();
		#endif
		
		swiWaitForVBlank();
	}

	return 0;
}
