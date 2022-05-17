/**
 * @file hello-triangle.c
 * @author Brais Solla G.
 * @brief PSP Hello Triangle example
 * @version 0.1
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <pspgu.h>
#include <pspgum.h>

#include "callbacks.h"
#include "vram.h"

PSP_MODULE_INFO("Hello triangle", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define BUF_WIDTH  (512)
#define SCR_WIDTH  (480)
#define SCR_HEIGHT (272)

static unsigned int __attribute__((aligned(16))) list[262144];


int main(int argc, char* argv[]){
    setupCallbacks();


    // Init GU
    sceGuInit();
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, (void*) 0, BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void*) 0x88000, BUF_WIDTH);
    sceGuDepthBuffer((void*)0x110000,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(0xc350,0x2710);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuFinish();
	sceGuSync(0,0);
	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);


    float vertices[] = {
        0.f, -.8f, 0.f, 0xff0000ff,
        -.8f, .8f, 0.f, 0xff00ff00,
        .8f, .8f,  0.f, 0xffff0000
    };

    uint32_t* colors = (uint32_t*) vertices;
    // Hacky stuff
    colors[3]  = 0xff0000ff;
    colors[7]  = 0xff00ff00;
    colors[11] = 0xffff0000;

    while(running()){
        sceGuStart(GU_DIRECT, list);

        sceGuClearColor(0);
        sceGuClearDepth(0);
        // glClear-ish
        sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();

        // Triangle vertices
        sceGuDrawArray(GU_TRIANGLES, GU_VERTEX_32BITF | GU_COLOR_8888, 3*3, 0, vertices);


        sceGuFinish();
        sceGuSync(0,0);

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }




    sceGuTerm();
    sceKernelExitGame();
    return 0;
}