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

// http://problemkaputt.de/gbatek.htm#ds3diomap
#define DISP3DCNT   *((vu16_t*) 0x4000060)
#define CLEAR_COLOR *((vu32_t*) 0x4000350)
#define CLEAR_DEPTH *((vu16_t*) 0x4000354)

#define GXFIFO_ADDR 0x4000400
#define GXFIFO      *((vu32_t*) GXFIFO_ADDR)

#define GXSTAT      *((vu32_t*) 0x4000600)

// Geometry Engine commands
#define CMD_UNPACKED(x) (((x) & 0xff))
#define CMD_ARG(x)      ((x & 0xffffffff))


#define CMD_NOP      CMD_UNPACKED(0x00)
#define CMD_MTXMODE  CMD_UNPACKED(0X10)
#define CMD_MTXPUSH  CMD_UNPACKED(0x11)
#define CMD_MTXPOP   CMD_UNPACKED(0x12)
#define CMD_MTXSTORE CMD_UNPACKED(0x13)



int main() {	
	videoSetMode(MODE_0_3D);

	powerOn(POWER_3D_CORE | POWER_MATRIX);

	
	while(1) {
		scanKeys();

		u16 keys = keysDown();
		if(keys & KEY_START) break;

		



		swiWaitForVBlank();
	}

	return 0;
}
