#include <SDL2/SDL.h>
#include "SDL_c.h"
#include "quads.h"
#include <SDL2/SDL_mixer.h>
#include "Line.h"
#include <vector>
class Game
{
private:
     renderer mrenderer;
     font mfont;
     bool running ;
     bool menu_running;
	bool game_running;
    float load_percent;
    Sprite bgTexture;
    SDL_Event event;
    Mix_Music* bg_music;
    Mix_Chunk* coin_collected_sound;
    Mix_Chunk* obstacle_collision;
    std::vector<Line> lines;
    struct gameAssets {
        Sprite obstacles[20];       //array of sprites for obstacles
        Sprite man;
        Sprite Coin;
        SDL_Rect man_sprite_position[28];
    }cur_game;


public:
    Game(renderer &main_renderer)
    {
        mrenderer=main_renderer;
        running = true;
		menu_running=true;
		game_running=false;

        load_percent = 0;
        char *log = mfont.load_file(mrenderer, "./misc/font.png");
        if (log != nullptr)
        {
            __debugbreak();
        }
        coin_collected_sound = Mix_LoadWAV("./misc/coin_collected.wav");
        if (!coin_collected_sound)
        {
            std::cout << "Error loading coin sound" << std::endl;
        }
		bg_music=Mix_LoadMUS("./misc/bg.wav");
        if (!bg_music)
        {
            std::cout << "Error loading background music" << std::endl;
        }
        if (!bg_music)
        {
            std::cout << "BG music not found" << std::endl;
            const char* err = Mix_GetError();

            __debugbreak();
        }
        if (coin_collected_sound == NULL)
        {
            __debugbreak();
        }
        bgTexture.load(mrenderer, "./misc/bg_scaled.png");
        if (((SDL_Texture*)bgTexture) == NULL)
        {
            const char * error = SDL_GetError();
            __debugbreak();
        }
        lines.reserve(sizeof(Line) * 3000);
    }
    void start_game()
    {
        Mix_Music* loading = Mix_LoadMUS("./misc/start.wav");
        
        Mix_PlayMusic(loading, -1);
        load_screen();
        while (game_running || menu_running)
        {
            if (menu_running)
                menu();
            Mix_HaltMusic();
            if (game_running)
                game();
			if(menu_running)
            Mix_PlayMusic(loading, -1);
        }
    }
    void menu();
    void game();

    void load_screen();
    void load_game_assets();


};
