
#ifdef __linux__ 
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL2\SDL.h>
#endif
#include "headers/defines.h"
#include "headers/SDL_c.h"
#include "headers/game.hpp"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window mwindow("Window",SCREEN_WIDTH ,SCREEN_HEIGHT );
    renderer mrenderer(mwindow, 0);
	Game game(mrenderer);
    game.start_game();
    return 0;
}
#ifdef __linux__
    void __debugbreak(){}
#endif