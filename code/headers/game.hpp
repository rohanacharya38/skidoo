#ifdef __linux__ 
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL2\SDL.h>
#endif
#include "SDL_c.h"
class Game
{
private:
     renderer mrenderer;
     font mfont;
     bool running ;
     bool menu_running;
	bool game_running;
    Sprite sprites[28];
    Sprite man_spritesheet;
    Sprite bgTexture;
    SDL_Event event;

public:
    Game(renderer &main_renderer)
    {
        mrenderer=main_renderer;
        running = true;
		menu_running=true;
		game_running=false;
        char *log = mfont.load_file(mrenderer, "./misc/font.png");
        if (log != nullptr)
        {
            __debugbreak();
        }
    }
    void start_game();
    void menu();
    void game();


};
