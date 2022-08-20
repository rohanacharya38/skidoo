
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "headers/defines.h"
#include "headers/SDL_classes.h"
#include "headers/game.hpp"

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR IN INITITALIZATOIN:SDL" << std::endl << SDL_GetError() << std::endl;
    };
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        std::cout << "ERROR IN INITITALIZATOIN:SDL-Mixer" << std::endl << Mix_GetError() << std::endl;
    }
    window mwindow("Window",SCREEN_WIDTH ,SCREEN_HEIGHT );
    renderer mrenderer(mwindow, 0);
	Game game(mrenderer);
    game.start_game();
    return 0;
}
#ifdef __linux__
    void __debugbreak(){}
#endif