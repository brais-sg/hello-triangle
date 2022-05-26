/**
 * @file main.cpp
 * @author Brais Solla G.
 * @brief Wii U Hello triangle example
 * @version 0.1
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <nn/ac.h>
#include <coreinit/memdefaultheap.h>
// GX2
#include <gx2/clear.h>
#include <gx2/draw.h>
#include <gx2/mem.h>
#include <gx2/utils.h>
// #include <gx2/shaders.h>

#include "shaders/basic.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
    nn::ac::ConfigIdNum configId;

    nn::ac::Initialize();
    nn::ac::GetStartupId(&configId);
    nn::ac::Connect(configId);

    
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, basic_VS.program, basic_VS.size);
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, basic_PS.program, basic_PS.size);




    nn::ac::Finalize();
    return 0;
}
