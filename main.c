#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

int main()
{
    uint32_t pixels[SCR_WIDTH * SCR_HEIGHT];
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("TestGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow(): %s\n", SDL_GetError());
        return  -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(framebuffer, SDL_BLENDMODE_BLEND);
    
    uint32_t player_x = 0, player_y = 0;
    uint32_t player_width = 20, player_height = 20;

    bool quit = false;
    while (!quit) {
         SDL_Event e;
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

        for (uint32_t y = player_y; y < player_y + player_height; y++) {
            for (uint32_t x = player_x; x < player_x + player_width; x++) {
                pixels[(y * SCR_WIDTH + x) % (SCR_WIDTH * SCR_HEIGHT)] = 0x0000FFAA;
            }
        }

        player_x++; player_y++;


        SDL_UpdateTexture(framebuffer, NULL, pixels, SCR_WIDTH * sizeof(uint32_t));
        SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
