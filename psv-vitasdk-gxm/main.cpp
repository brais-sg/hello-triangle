/**
 * @file main.cpp
 * @author Brais Solla G.
 * @brief LIBGXM (PSVITA) Hello Triangle
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// VITASDK
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>

#include <psp2/gxm.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>

#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 544
#define DISPLAY_STRIDE 1024
#define DISPLAY_BUFFER_COUNT 2
#define DISPLAY_COLOR_FORMAT SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT SCE_DISPLAY_PIXELFORMAT_A8B8G8R8
#define MAX_PENDING_SWAPS (DISPLAY_BUFFER_COUNT - 1)

struct display_queue_callback_data {
	void *addr;
};

static void display_queue_callback(const void *callbackData);

// Before doing nothing: Take a look at xerpi's gxmfun repository
int main(int argc, char* argv[]){
    SceGxmInitializeParams gxm_init_params;
    memset(&gxm_init_params, 0, sizeof(gxm_init_params));

    gxm_init_params.flags = 0;
    gxm_init_params.displayQueueMaxPendingCount = MAX_PENDING_SWAPS;
    gxm_init_params.displayQueueCallback = display_queue_callback;
    gxm_init_params.displayQueueCallbackDataSize = sizeof(struct display_queue_callback_data);
    gxm_init_params.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

    sceGxmInitialize(&gxm_init_params);

    // Initialize GPU rings
    




    sceKernelExitProcess(0);
    return 0;
}