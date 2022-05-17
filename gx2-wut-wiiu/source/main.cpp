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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
    nn::ac::ConfigIdNum configId;

    nn::ac::Initialize();
    nn::ac::GetStartupId(&configId);
    nn::ac::Connect(configId);


    // TODO: Everything




    nn::ac::Finalize();
    return 0;
}
