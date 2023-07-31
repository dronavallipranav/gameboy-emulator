#include <SDL.h>
#include <stdbool.h>
#include <cpu.h>

//Change linkage based on OS
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #define GL_SILENCE_DEPRECATION
#else
    #include <GL/gl.h>
#endif

int main() {

    Z80_State cpu;
    initCPU(&cpu);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) { 
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Event e;
    bool quit = false;
    uint8_t DINextInstr = 0;
    uint8_t EINextInstr = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        //TO DO: FETCH/EXECUTE NEXT INSTR

        if(cpu.DI_status){
            DINextInstr++;
            if(cpu.DI_status && DINextInstr == 2){
                DINextInstr = 0;
                cpu.DI_status = false;
                cpu.interrupt_status = false;
            }
        }
        if(cpu.EI_status){
            EINextInstr++;
            if(cpu.EI_status && EINextInstr == 2){
                EINextInstr = 0;
                cpu.EI_status = false;
                cpu.interrupt_status = true;
            }
        }

        if(!cpu.Stop){
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
        }else{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
        } 
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
