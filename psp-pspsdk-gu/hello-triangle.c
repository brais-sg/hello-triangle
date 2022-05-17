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

struct Vertex {
 	float x,y,z;
};

static unsigned int __attribute__((aligned(16))) list[262144];


int main(int argc, char* argv[]){
    setupCallbacks();

    // Framebuffer pointer 0
    void* fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);

    // Framebuffer pointer 1
	void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);

    // Z Buffer pointer
	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

    // Init GU
    sceGuInit();
    sceGuStart(GU_DIRECT, list);

    // Create buffers
    sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
    sceGuDepthBuffer(zbp,BUF_WIDTH);
    // PSP uses a virtual coordinate system [0-4096], translate to the middle of the screen
    sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(65535,0);

    // Show full screen
    sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuFinish();

    sceGuSync(0,0);
	sceDisplayWaitVblankStart();
    // Turn on display
	sceGuDisplay(GU_TRUE);




    while(running()){
        sceGuStart(GU_DIRECT, list);

        // Clear color and depth buffers
        sceGuClearColor(0);
        sceGuClearDepth(0);

        sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();

        // Red?
        sceGuColor(0xff0000ff);

        // Allocate memory for the triangle vertices
        struct Vertex* vertices = (struct Vertex*) sceGuGetMemory(3 * sizeof(struct Vertex));
        // Generate the triangle
        vertices[0].x = SCR_WIDTH / 2;
		vertices[0].y = 20;
		vertices[0].z = 0;
		vertices[1].x = (SCR_WIDTH / 6);
		vertices[1].y = (SCR_HEIGHT / 2) + 80;
		vertices[1].z = 0;
		vertices[2].x = SCR_WIDTH - (SCR_WIDTH / 6);
		vertices[2].y = (SCR_HEIGHT / 2) + 80;
		vertices[2].z = 0;

        sceGuDrawArray(GU_TRIANGLES, GU_VERTEX_32BITF | GU_TRANSFORM_2D, 3, 0, vertices);


        sceGuFinish();
        sceGuSync(0,0);

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }




    sceGuTerm();
    sceKernelExitGame();
    return 0;
}