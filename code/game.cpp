#include <SDL2\SDL.h>
#include "headers/game.hpp"
#include "headers/threading.h"
#include <thread>
#include "headers/quads.h"
#include "headers/Line.h"
#include <vector>
/*...................*/

extern std::vector<Line> lines;
//this is a global variable in line.cpp file that is used in this code;

/*...................*/
bool assets_loaded = false;

#define NO_OF_OBSTACLES 16

struct gameAssets {
    Sprite obstacles[20];       //array of sprites for obstacles
	Sprite man;
    Sprite Coin;
    SDL_Rect man_sprite_position[28];
}cur_game;

int load_man_pngs(void *arg)
{
    cur_game.man_sprite_position[0].x = 0;
    cur_game.man_sprite_position[0].y = 0;
    cur_game.man_sprite_position[0].h = 350;
    cur_game.man_sprite_position[0].w = 145;
    for (int i = 1; i < 28; i++)
    {
        cur_game.man_sprite_position[i].h = MAN_HEIGHT;
        cur_game.man_sprite_position[i].w = MAN_WIDTH;
        cur_game.man_sprite_position[i].x = cur_game.man_sprite_position[i-1].x+ cur_game.man_sprite_position[i].w+65;
        cur_game.man_sprite_position[i].y = 0;
    }
	

	
    Sprite* man_sprite = &cur_game.man;
    renderer *add_mrenderer = (renderer*)arg;   //adress of renderer passed through thread creation
    man_sprite->load(*add_mrenderer, "./misc/man.png");
    //cur_game.man_sprite_position = { (SCREEN_WIDTH / 2) - 75,(SCREEN_HEIGHT / 2) - 50,210,350 };
    cur_game.Coin.load(*add_mrenderer, "./misc/coin.png");
    
	/*Now load all the obstacles in sprites array*/
    char data[100];
    for (int i = 1; i <NO_OF_OBSTACLES ; i++)
    {
        sprintf_s(data, "./misc/%d.png", i);
		cur_game.obstacles[i].load(*add_mrenderer, (data));
    }

	/*................*/
	//these function write to global vector variable line
    generateLines();
    generateObstacles((cur_game.obstacles));
    generateCoins(cur_game.Coin);
    assets_loaded = true;
	/******************/
    return 0;
}
void Game::start_game()
{

    //quad environment_quad(0.0f, 0.0, 0.0, SCREEN_WIDTH, 500 + 0.0, { 2, 41, 2, 255 });
    //road_rect will change 

    /*quad road_quad(SCREEN_WIDTH / 2 - 100.0, SCREEN_HEIGHT, 0.0, 100, 1500 + 0.0, { 107,107,107, 255 });*/


    bgTexture.load(mrenderer, "./misc/bg.jpg");
    bgTexture.position_in_screen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	

    char score_display[100];
    char coin_display[100];
	
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

    thrd_t man_loading_thrd;
    thrd_create(&man_loading_thrd, load_man_pngs, reinterpret_cast<void*>(&mrenderer));
    /*.............................*/
    /*............................*/
    int anim_frame = 0;
MENU:

    bgTexture.position_in_screen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    while (menu_running)
    {
        mrenderer.set_color(0, 0, 0, 255);
        mrenderer.clear();
        bgTexture.render(nullptr);
        mfont.render_string(mrenderer, "START", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
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
    /*..........................*/
    bgTexture.position_in_screen = { -1500,-300,6000,792 };

    /*.........................*/
    /*Joining game assets loading thread here to ensure everything is loaded properly*/
    /*.........................*/
    constexpr int frames = 27;
    float animDuration = 0.45;

    /*******************************************/
	/*quads for game*/
    Quad env;
	Quad side;
	Quad road_lane;

	/*...........................................*/




	/*.........................................*/
    if (!assets_loaded)
    {
        thrd_join(man_loading_thrd, nullptr);
}
    cur_game.man.position_in_screen = { SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 50,MAN_WIDTH,MAN_HEIGHT };

    /*.........................*/
    int N_lines = lines.size();
    for (int i = 0; i < 2500; i++)
    {
        lines[i%N_lines].collected = false;
    }
    int zpos = 0;
    int xpos = 0;
    int score = 0;
    int coin_collected = 0;
    int isAlive = true;
    int speed = 200;
    float CollideTime = 0;
	
    //----------------------gravity----------------------------------//
    float groundHeight = (height / 2)-75;
    float gravity = 3.0f;
    bool isJumping = false;
    float jmpSpeed = 300.0f;
	/***************************/

	
    /***************************/
    
  constexpr  float colliderX = width / 2 - 40;
   constexpr float colliderY = height / 2 + 200;

    /***************************/

    int frame_increment = 1;

    while (game_running)
    {

        mrenderer.set_color(0, 0, 0, 255);
        mrenderer.clear();
        //bgTexture.position_in_screen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        /*rendering background texture(sky)*/
        bgTexture.render(nullptr);

        /*rendering the fields in the side of road*/
        //environment_quad.render(mrenderer);
        //Color environment = (n / 5) % 2 ? Color(2, 41, 2) : Color(3, 43, 3);
        //Color roadside = (n / 3) % 2 ? Color(253, 227, 8) : Color(0, 0, 0);
        //Color road = (n / 12) % 2 ? Color(107, 107, 107) : Color(109, 109, 109);
        /*rendering the road itself*/
        //road_quad.render(mrenderer);
        /*...............................................................*/
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
                case SDLK_UP:
                    if (cur_game.man.position_in_screen.y >= groundHeight) {
                        isJumping = true;
                        cur_game.man.move(0, -jmpSpeed);

                    }
                    break;
                case SDLK_RIGHT:
                    if (cur_game.man.position_in_screen.x < colliderX + 200)
                    {
                        cur_game.man.move(30, 0);
                    }

                    break;
                case SDLK_LEFT:
                    if (cur_game.man.position_in_screen.x > colliderX - 300)
                    {
                        cur_game.man.move(-30, 0);
                    }
                    break;
                default:
                    break;

                }

            }
            }
        }
        if (cur_game.man.position_in_screen.y < groundHeight && isJumping == true) {
            cur_game.man.move(0, gravity);
        }
        else
            isJumping = false;
        speed = 50 + (score > 300) * 25;
        animDuration = 0.45 - (score > 300) * 0.1;

        if (isAlive)
            zpos += speed;

        if (zpos >= N_lines * segL)
            zpos -= N_lines * segL;
        if (zpos < 0)
            zpos += N_lines * segL;


        int startPos = zpos / segL;
        bgTexture.move(-lines[startPos].getCurve() * 2, 0);
        //if (isAlive)  //TODO::
        //    score = clockForScore.getElapsedTime().asMilliseconds() / 20;
        if (startPos + 300 > N_lines)
        {
            zpos = 0;
            startPos = 0;
            bgTexture.position_in_screen = { -1500,-300,6000,792 };

        }
        sprintf_s(score_display, "Score: %d", score);
        sprintf_s(coin_display, "Coins:%d", coin_collected);
        mfont.render_string(mrenderer, score_display, 10, 10);
        mfont.render_string(mrenderer, coin_display, 10, 30);
        //------COLLISION TEST-------//
       /* if (isAlive)
            if (SDL_HasIntersectionF( & (lines[startPos + 10].getObstacleBounds()), &(cur_game.man.position_in_screen)))
            {
                isAlive = false;
            }*/
            //----------X, Y, Z POSITIONs INITIALIZE---------------------//
        float x = 0, dx = 0;
        int maxy = height;

        //--------THIS IS THE MAIN LOOP FOR RUNNING--------//
        for (int n = startPos; n < startPos + 300; n++)
        {
            Line& l = lines[n % N_lines]; //Line &l is reference to each line being drawn

            l.projectToScreen(xpos - x, 1500, zpos - (n >= N_lines ? N_lines * segL : 0));

            x += dx;
            dx += l.getCurve();

            if (l.getY() >= maxy)
                continue;
            Line p;
            if (n != 0 && n != 1)
                p = lines[(n - 1) % N_lines]; //previous line

   /*environment quads*/
            env.render(mrenderer, { 2,41,2,255 }, 0, p.getY(), SCREEN_WIDTH, 0, l.getY(), SCREEN_WIDTH);
            side.render(mrenderer, { 253,227,8,255 }, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2);

            road_lane.render(mrenderer, { 107,107,107,255 }, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW());

        }


        //-------DRAWING OBSTACLES AND COINS----------//
        for (int n = startPos + 300; n > startPos; n--)
        {
            lines[n % N_lines].drawSprite(mrenderer);
            lines[n % N_lines].drawCoin(mrenderer);
        }

        //------COIN TOUCH TEST----//
        if (SDL_HasIntersectionF(&(lines[startPos + 5].coin_texture), &cur_game.man.position_in_screen) && (lines[startPos + 5].collected == false))
        {
            coin_collected++;
            lines[startPos + 5].collected = true;
        }
        if (SDL_HasIntersectionF(&(lines[startPos + 10].getObstacleBounds()), &(cur_game.man.position_in_screen)))
        {
            isAlive = false;
        }

        //cur_game.man.render(&cur_game.man_sprite_position);
        if (isJumping)
        {
            cur_game.man.render(&cur_game.man_sprite_position[27]);
        }
        else
        {
            if (anim_frame > 27)
                frame_increment = -1;
            else if (anim_frame < 0)
                frame_increment = 1;
            anim_frame += frame_increment;
            cur_game.man.render(&cur_game.man_sprite_position[anim_frame]);
        }
        SDL_RenderDrawRectF(mrenderer, &cur_game.man.position_in_screen);
        //SDL_RenderDrawLine(mrenderer,colliderX,colliderY,SCREEN_HEIGHT,SCREEN_WIDTH);
        mrenderer.render();
        if (!isAlive)
        {
            menu_running = true;
        game_running = false;
    }
    }
    bgTexture.position_in_screen = { 0,0,SCREEN_HEIGHT,SCREEN_WIDTH };
            if (menu_running)
                goto MENU;

}
