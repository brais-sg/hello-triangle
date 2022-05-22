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

int main(int argc, char* argv[]){
    // Just wait for like 30 seconds and exit

    for(int i = 0; i < 30; i++){
        sceKernelDelayThread(1000000);
    }

    sceKernelExitProcess(0);
    return 0;
}