#include <SDL.h>
#include "SDL_c.h"
class Game
{
private:
     renderer mrenderer;
     font mfont;
public:
    Game(renderer &main_renderer)
    {
        mrenderer=main_renderer;
        
        char *log = mfont.load_file(mrenderer, "./misc/font.png");
        if (log != nullptr)
        {
            __debugbreak();
        }
    }
    void start_game();
};
