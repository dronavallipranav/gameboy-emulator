#include <SDL.h>
#include <stdbool.h>
#include <cpu.h>
#include <mmu.h>

// Change linkage based on OS
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#define GL_SILENCE_DEPRECATION
#else
#include <GL/gl.h>
#endif

static int scale = 4;

static unsigned long tile_colors[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

void displayTile(Z80_State *cpu, SDL_Surface *surface, uint16_t start, uint16_t tile, int x, int y)
{
    SDL_Rect rect;

    for (int ytile = 0; ytile < 16; ytile++)
    {
        uint8_t firstByte = read_byte(cpu->mmu->ppu, cpu->mmu, start + (tile * 16) + ytile);
        uint8_t secondByte = read_byte(cpu->mmu->ppu, cpu->mmu, start + (tile * 16) + ytile + 1);

        for (int bit = 7; bit >= 0; bit--)
        {
            uint8_t high = !!(firstByte & (1 << bit)) << 1;
            uint8_t lo = !!(secondByte & (1 << bit)) << 1;

            uint8_t color = high | lo;

            rect.x = x + ((7 - bit) * scale);
            rect.y = y + ((ytile / 2) * scale);
            rect.w = scale;
            rect.h = scale;

            SDL_FillRect(surface, &rect, tile_colors[color]);
        }
    }
}

void updateWindow(Z80_State *cpu, SDL_Window *window)
{
    SDL_Rect rect;

    int xDraw = 0;
    int yDraw = 0;
    int tileNum = 0;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    uint16_t addr = 0x8000;

    for (int y = 0; y < 24; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            displayTile(cpu, surface, addr, tileNum, xDraw, yDraw);
            tileNum++;
            xDraw += 8;
        }
        xDraw = 0;
        yDraw += 8;
    }

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
}

int main()
{

    Z80_State cpu;
    initCPU(&cpu);
    load_memory(cpu.mmu, "/Users/pranavdronavalli/Downloads/pokemon_red.gb");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // TO DO: FETCH/EXECUTE NEXT INSTR

        if (!cpu.Stop)
        {
            updateWindow(&cpu, window);
        }
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
