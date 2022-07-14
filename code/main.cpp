
#include <SDL.h>
#include "headers/SDL_c.h"
#include "headers/game.hpp"
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window mwindow("Window", 640, 480);
    renderer mrenderer(mwindow, 0);
	Game game(mrenderer);
    game.start_game();
    return 0;
}
