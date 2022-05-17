/**
 * @file main.cpp
 * @author Brais Solla G.
 * @brief SDL2 OpenGL Legacy (1.3) Hello triangle demo
 * @version 0.1
 * @date 2022-05-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/gl.h>

SDL_Window* window;
SDL_GLContext gContext;


#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
        exit(-1);
    }

    // Set OpenGL ES 2.0 context 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create OpenGL Window
    window = SDL_CreateWindow("hello triangle (OpenGL Legacy (1.3) + SDL2)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(!window){
        fprintf(stderr, "Cannot create Window: %s\n", SDL_GetError());
        exit(-2);
    }

    gContext = SDL_GL_CreateContext(window);
    if(!gContext){
        fprintf(stderr, "Cannot create OpenGL Context: %s\n", SDL_GetError());
        exit(-3);
    }

    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Vendor  : %s\n", glGetString(GL_VENDOR));
    printf("Version : %s\n", glGetString(GL_VERSION));


    SDL_Event e;
    bool running = true;

    // prepare_triangle();

    GLfloat vertices[] = {
        0.f, .8f, 0.f,
        -.6f, -.6f, 0.f,
        .6f, -.6f, 0.f
    };

    GLfloat colors[] = {
        1.f, 0.f, 0.f, 1.f, // RED
        0.f, 1.f, 0.f, 1.f, // GREEN
        0.f, 0.f, 1.f, 1.f  // BLUE
    };


    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        // draw_triangle();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glColorPointer(4, GL_FLOAT, 0, colors);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        
        SDL_GL_SwapWindow(window); 
        SDL_Delay(16); // Limit to ~60fps
    }

    printf("Exiting...\n");

    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(window);

    return 0;
}