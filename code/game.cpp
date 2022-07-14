#include "headers/game.hpp"

void Game::start_game()
{
        //2 while loops for render and input with loop vairable
    bool running = true;
    SDL_Event event;
    while (running)
    {

        mrenderer.set_color(0,0,0,255);
        mrenderer.clear();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        mfont.render_string(mrenderer, string("Hello World!", 12), 200, 200);
        mrenderer.render();
    }
}
