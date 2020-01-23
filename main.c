#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

typedef struct {
    uint32_t x, y;
} v2;

typedef struct {
    enum {ENTITY_NONE, ENTITY_PLAYER} type;
    v2 pos;
    uint32_t width, height;
} entity_t;


int main()
{
    uint32_t pixels[SCR_WIDTH * SCR_HEIGHT];
    entity_t player = {.type = ENTITY_PLAYER, .pos = {0, 0}, .width = 20, .height = 20};
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("TestGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow(): %s\n", SDL_GetError());
        return  -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCR_WIDTH, SCR_HEIGHT);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(framebuffer, SDL_BLENDMODE_BLEND);
    
    bool quit = false;
    while (!quit) {
         SDL_Event e;
         uint32_t ticks, passedTicks;
         ticks = SDL_GetTicks();
         while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                 quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
        }

        memset(pixels, 0, SCR_WIDTH * SCR_HEIGHT * sizeof(uint32_t));
        SDL_RenderClear(renderer);

        for (uint32_t y = player.pos.y; y < player.pos.y + player.height; y++) {
            for (uint32_t x = player.pos.x; x < player.pos.x + player.width; x++) {
                pixels[(y * SCR_WIDTH + x) % (SCR_WIDTH * SCR_HEIGHT)] = 0x0000FFFF;
            }
        }

        player.pos.x += 1;
        player.pos.y += 1;

        SDL_UpdateTexture(framebuffer, NULL, pixels, SCR_WIDTH * sizeof(uint32_t));
        SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
        SDL_RenderPresent(renderer);

        passedTicks = SDL_GetTicks() - ticks;
        if (passedTicks < 20) {
            SDL_Delay(20 - passedTicks);
        }
        printf("%u\n", 1000 / passedTicks);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
