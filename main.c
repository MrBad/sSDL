#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"

#define SCR_WIDTH 876
#define SCR_HEIGHT 578

typedef struct {
    uint32_t x, y;
} v2;

typedef struct {
    enum {ENTITY_NONE, ENTITY_PLAYER} type;
    v2 pos;
    uint32_t width, height;
} entity_t;

typedef struct {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t imageOffs;
} __attribute__((packed)) bmp_header_t;

typedef struct {
    uint32_t headerSize;
    int32_t imageWidth;
    int32_t imageHeight;
    int16_t colorPlanes;
    int16_t bitsPerPixel;
    uint32_t extra[6];
} __attribute__((packed)) bmp_info_t;

typedef struct {
    uint8_t r, g, b;
} __attribute__ ((packed)) rgb_t;

// internal representation //
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t bufSize;
    uint8_t data[];
} bmp_t;


bmp_t *loadBMP(const char *fileName)
{
    bmp_t *ret = NULL;
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        perror("fopen()");
        return NULL;
    }
    bmp_header_t bmp_header;
    bmp_info_t bmp_info;
    size_t sz = 0;
    sz = fread(&bmp_header, 1, sizeof(bmp_header), fp);
    if (sz != sizeof(bmp_header)) {
        perror("fread() bmp_header");
        fclose(fp);
        return NULL;
    }
    sz = fread(&bmp_info, 1, sizeof(bmp_info), fp);
    if (sz != sizeof(bmp_info)) {
        perror("fread() bmp_info");
        fclose(fp);
        return NULL;
    }

    if (bmp_info.bitsPerPixel != 24) {
        perror("Unhandled %d bits per pixel. Please use 24 bits per pixels");
        fclose(fp);
        return NULL;
    }

    uint32_t bufSize = bmp_info.imageWidth * bmp_info.imageHeight * bmp_info.bitsPerPixel / 8;
    ret = malloc(sizeof(bmp_t) + bufSize);
    if (!ret) {
        perror("malloc()");
        fclose(fp);
        return NULL;
    }
    sz = fread(ret->data, 1, bufSize, fp);
    if (sz != bufSize) {
        perror("fread()");
        free(ret);
        fclose(fp);
        return NULL;
    }

    fclose(fp);

    ret->width = bmp_info.imageWidth;
    ret->height = bmp_info.imageHeight;
    ret->bufSize = bufSize;

    return ret;
}

int main()
{
    
    bmp_t *eveMiau = loadBMP("eve-miau2.bmp");
    if (!eveMiau) {
        return -1;
    }
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
    SDL_Texture *framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCR_WIDTH, SCR_HEIGHT);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(framebuffer, SDL_BLENDMODE_BLEND);
    
    bool quit = false;
    uint32_t framesTicks = 0, numFrames = 0;
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

        uint8_t *p = eveMiau->data;
        for (int y = SCR_HEIGHT-1; y >= 0; y--) {
            for (int x = 0; x < SCR_WIDTH; x++) {
                pixels[y * SCR_WIDTH + x] = *(p) | *(p + 1)<<8 | *(p + 2) << 16 | 0xFF << 24;
                p += 3;
            }
        }

        for (uint32_t y = player.pos.y; y < player.pos.y + player.height; y++) {
            for (uint32_t x = player.pos.x; x < player.pos.x + player.width; x++) {
                pixels[(y * SCR_WIDTH + x) % (SCR_WIDTH * SCR_HEIGHT)] = 0xCC0000FF;
            }
        }
        player.pos.x += 1;
        player.pos.y += 1;

        SDL_RenderClear(renderer);
        SDL_UpdateTexture(framebuffer, NULL, pixels, SCR_WIDTH * sizeof(uint32_t));
        SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
        SDL_RenderPresent(renderer);

   
        uint32_t currTicks = SDL_GetTicks();
        if (currTicks - framesTicks > 1000) {
            printf("FPS: %d\n", numFrames);
            numFrames = 0;
            framesTicks = currTicks;
        }
        numFrames++;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    free(eveMiau);
    return 0;
}

