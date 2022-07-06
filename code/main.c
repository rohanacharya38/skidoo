//includes

#include "custom_functions.h"
#include <SDL.h>
#include <stdint.h>

//defines
#define SCREEN_WIDTH 1350
#define SCREEN_HEIGHT 700
#define RECT_COUNT 190
#define sWIDTH 6
#define PADDING 1

//emum for bool
typedef enum
{
    false,
    true
} bool;
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *img_texture = load_img("../misc/font.png", renderer);
 
    bool is_running = true;
    SDL_Event event;
    while (is_running)
    {
       
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                is_running = false;
        }
    }

    return 0;
}
