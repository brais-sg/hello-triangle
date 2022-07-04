/**
 * @file main.cpp
 * @author Brais Solla G.
 * @brief SDL2 OpenGL ES 2.0 Hello triangle demo
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
#include <stdbool.h>

#include <android/log.h>

#include "SDL.h"
#include "SDL_opengles2.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

SDL_Window* window;
SDL_GLContext gContext;

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

const char* vShaderSource = 
    "attribute vec3 v_Input;\n"
    "attribute vec4 c_Input;\n"
    "varying vec4 colorFragment;\n"
    "void main(){\n"
    "   colorFragment  = c_Input;\n"
    "   gl_Position = vec4(v_Input.xyz, 1.0);\n"
    "}\n";

const char* fShaderSource = 
    "varying vec4 colorFragment;\n"
    "void main(){\n"
    "   gl_FragColor = colorFragment;\n"
    "};\n";

int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
        exit(-1);
    }

    // Set OpenGL ES 2.0 context 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create OpenGL Window
    window = SDL_CreateWindow("hello triangle (OpenGL ES 2.0 + SDL2)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if(!window){
        fprintf(stderr, "Cannot create Window: %s\n", SDL_GetError());
	__android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "Cannot create Window: %s\n", SDL_GetError());
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

    __android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "Renderer: %s\n", glGetString(GL_RENDERER));
    __android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "Vendor:   %s\n", glGetString(GL_VENDOR));
    __android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "Version:  %s\n", glGetString(GL_VERSION));

    SDL_Event e;
    bool running = true;

    // prepare_triangle();

    // Maybe check for errors during shader compiling/linking?
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSource, NULL);
    glCompileShader(vertexShader);
    
    GLint vertex_compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);
	
	if (vertex_compiled != GL_TRUE){
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(vertexShader, 1024, &log_length, message);
		__android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "VSHADER ERR: %s\n", message);
	}
	

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    GLint fragment_compiled;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
	
	if (fragment_compiled != GL_TRUE){
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);
		__android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "FSHADER ERR: %s\n", message);
	}
	

    GLuint shaderProgram = glCreateProgram();
    
    glBindAttribLocation(shaderProgram, 0, "v_Input");
    glBindAttribLocation(shaderProgram, 1, "c_Input");
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    
    
    GLint program_linked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE){
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(shaderProgram, 1024, &log_length, message);
		__android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI", "LINK ERR: %s\n", message);
	}

    
    glUseProgram(shaderProgram);

    __android_log_print(ANDROID_LOG_DEBUG, "HELLOTRI","ShaderProgram: %d\n", (int) shaderProgram);

    GLint positionAttrib = glGetAttribLocation(shaderProgram, "v_Input");
    GLint colorAttrib    = glGetAttribLocation(shaderProgram, "c_Input");
  

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

    // If colorAttrib is disabled, triangle will be white
    glVertexAttrib4f(colorAttrib, 1.f, 1.f, 1.f, 1.f);

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        // draw_triangle();

        glUseProgram(shaderProgram);

        glEnableVertexAttribArray(positionAttrib);
        glEnableVertexAttribArray(colorAttrib);

        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 0, colors);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(positionAttrib);
        glDisableVertexAttribArray(colorAttrib);

        SDL_GL_SwapWindow(window); 
        SDL_Delay(16); // Limit to ~60fps
    }

    printf("Exiting...\n");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glDeleteShader(shaderProgram);

    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(window);

    return 0;
}
