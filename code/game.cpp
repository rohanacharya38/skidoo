#include "headers/game.hpp"
#include "headers/threading.h"
#include <thread>
renderer* main_renderer;
struct gameAssets {
    Sprite obstacles[15];       //array of sprites for obstacles
	Sprite man;
};
SDL_Rect man_sprite_position[28];

auto load_man_pngs(void *arg)
{
    man_sprite_position[0].x = 0;
    man_sprite_position[0].y = 0;
    man_sprite_position[0].h = 350;
    man_sprite_position[0].w = 210;
    for (int i = 1; i < 28; i++)
    {
        man_sprite_position[i].h = 350;
        man_sprite_position[i].w = 210;
        man_sprite_position[i].x = man_sprite_position[i-1].x+ man_sprite_position[i].w;
        man_sprite_position[i].y = 0;
    }
	
    gameAssets* p = reinterpret_cast<gameAssets*>(arg);
	
    Sprite* man_sprite = & (p->man);
    man_sprite->load(*main_renderer, "./misc/man.png");
    if (!(p->man))
    {
        __debugbreak();
    }
    Sprite* obs = p->obstacles;
	/*Now load all the obstacles in sprites array*/
    char data[100];
    for (int i = 0; i <15 ; i++)
    {
        sprintf_s(data, "./misc/%d.png", i + 1);
		obs[i].load(*main_renderer, (data));
    }
    
    return 0;
}
void Game::start_game()
{
	
    main_renderer = &mrenderer;
    bgTexture.load(mrenderer, "./misc/bg.jpg");
    if (((SDL_Texture*)bgTexture) == NULL)
    {
        std::string error = SDL_GetError();
        return;
    }

	//in the next block

	/*...........................*/
    /*NO MANS LAND ,UNSAFE STUFFS HERE*/
	/*I am using threads to load all the sprites required so that the start time of our program is super low*/
	/*our program can do other stuffs like rendering menu while the loading of sprites are complete, in this way we can save valuable time */
    /*...........................*/
	
    gameAssets cur_game;
    thrd_t man_loading_thrd;
    thrd_create(&man_loading_thrd, load_man_pngs, reinterpret_cast<void*>(&cur_game));
	/*.............................*/
	/*............................*/
    int anim_frame= 0;
        MENU:
     
    while (menu_running)
    {
        mrenderer.set_color(0, 0, 0, 255);
		mrenderer.clear();
        bgTexture.render();
        mfont.render_string(mrenderer, "START", SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
			    case SDL_QUIT:
				menu_running = false;
                game_running = false;
				break;
                case SDL_MOUSEBUTTONDOWN: 
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (event.button.x > SCREEN_WIDTH / 2 - 50 && event.button.x < SCREEN_WIDTH / 2 + 50 && event.button.y > SCREEN_HEIGHT / 2 - 50 && event.button.y < SCREEN_HEIGHT / 2 + 50)
                        {
                            game_running = true;
                            menu_running = false;
                        }
                    }
                    break; 
                }
            }
        }
		mrenderer.render();
		
		
    }
	/*.........................*/
	/*Joining game assets loading thread here to ensure everything is loaded properly*/
	/*.........................*/
    thrd_join(man_loading_thrd,nullptr);
    /*.........................*/
    /*.........................*/
	
    while (game_running)
    {
        mrenderer.set_color(0,0,0,255);
        mrenderer.clear();
        //bgTexture.render();
 
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
				game_running = false;
                menu_running = false;
				break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym)
                {
					case SDLK_ESCAPE:
						game_running = false;
                        menu_running = true;
						break;
						case SDLK_SPACE:
                            mfont.render_string(mrenderer, "man is jumping", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
							break;
					
                }
				
            }
            }
        }
        cur_game.man.render(&man_sprite_position[anim_frame],nullptr);
        mrenderer.render();
        
            SDL_Rect pos = { SCREEN_WIDTH/2,100,200,200 };
            cur_game.obstacles[5].render(nullptr, nullptr);
        anim_frame++;
		
            if (anim_frame > 14)
                anim_frame= 0;
    }
            if (menu_running)
                goto MENU;

}
