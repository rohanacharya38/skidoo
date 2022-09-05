#include <SDL2/SDL.h>
#include "headers/game.hpp"
#include <thread>
#include "headers/quads.h"
#include "headers/Line.h"
#include "headers/defines.h"
#include <vector>
#include <fstream>

#if !(SDL_MAJOR_VERSION==2 && SDL_MINOR_VERSION==0 && SDL_PATCHLEVEL==22)
bool SDL_HasIntersectionF(const SDL_FRect *A ,const SDL_FRect *B)
{
SDL_Rect P = { (int)(A->x),(int)(A->y),(int)(A->w),(int)(A->h) };
SDL_Rect Q = { (int)(B->x),(int)(B->y),(int)(B->w),(int)(B->h) };
return SDL_HasIntersection(&P, &Q);
}
#endif
void sort(float* arr, int size);
bool gameCallSDL_HasIntersectionF(SDL_FRect* A, SDL_FRect* B)
{
        if (isnan(A->w)||isnan(A->h)||isnan(B->w)||isnan(B->h))
        {
            //printf("nan");
            A->w = 0;
			A->h = 0;
			return false;   
        }
    return SDL_HasIntersectionF(A, B);
}

/*...................*/
//this is a global variable in line.cpp file that is used in this code;
/*...................*/

#define NO_OF_OBSTACLES 16
void Game::game()
{
    float anim_frame = 0;
    Mix_PlayMusic(bg_music, -1);
    Mix_VolumeMusic(20);
    bgTexture.position_in_screen = { -1500,0,6000,SCREEN_HEIGHT };
    /*.........................*/
    /*Joining game assets loading thread here to ensure everything is loaded properly*/
    /*.........................*/
    constexpr int frames = 15;
    float animDuration = 0.45;

    /*******************************************/
    /*quads for game*/
    Quad env(Texture(mrenderer, "./misc/grass.png",  nullptr));
    Quad side;
    Quad road_lane;

    cur_game.man.position_in_screen = { SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 50,145,350 };

    /*.........................*/
    int N_lines = 2500;
    for (int i = 0; i < 2500; i++)
    {
        lines[i % N_lines].collected = false;
    }
    int zpos = 0;
    int xpos = 0;

    bool isAlive = true;
    int speed = 200;
    float CollideTime = 0;
    //----------------------gravity----------------------------------//
    float groundHeight = (height / 2) - 75;
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

        mrenderer.clear(0, 0, 0, 255);
        //bgTexture.position_in_screen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        /*rendering background texture(sky)*/
        bgTexture.render(nullptr);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                game_running = false;
                menu_running = false;
                return;
                break;


            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    game_running = false;
                    menu_running = false;
                    break;
                case SDLK_SPACE:
                case SDLK_UP:
                    if (cur_game.man.position_in_screen.y >= groundHeight) {
                        isJumping = true;
                        cur_game.man.move(0, -jmpSpeed);
                    }
                    break;
                case SDLK_RIGHT:

                    if (cur_game.man.position_in_screen.x < colliderX + 300)
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

        score += 0.05 + 0.5 * (score > 300);
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
            env.render(mrenderer, { 110,250,144,255 }, 0, p.getY(), SCREEN_WIDTH, 0, l.getY(), SCREEN_WIDTH);
            if (n % 2 == 0)
                side.render(mrenderer, { 255,255,255,255 }, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2);
            else
                side.render(mrenderer, { 255,255,0,255 }, p.getX(), p.getY(), p.getW() * 1.2, l.getX(), l.getY(), l.getW() * 1.2);
            road_lane.render(mrenderer, { 76,70,50,255 }, p.getX(), p.getY(), p.getW(), l.getX(), l.getY(), l.getW());

        }


        //-------DRAWING OBSTACLES AND COINS----------//
        for (int n = startPos + 300; n > startPos; n--)
        {
            lines[n % N_lines].drawSprite(mrenderer);
            lines[n % N_lines].drawCoin(mrenderer);
        }

        //------COIN TOUCH TEST----//

        if (gameCallSDL_HasIntersectionF(&(lines[startPos + 11].coin_texture), &cur_game.man.position_in_screen) && (lines[startPos + 11].collected == false))
        {
            Mix_PlayChannel(-1, coin_collected_sound, 0);
            coin_collected += 1;
            lines[startPos + 11].collected = true;
        }
        if (gameCallSDL_HasIntersectionF((&lines[startPos + 11].texture_rect), &(cur_game.man.position_in_screen)))
        {
             isAlive = false;
        }
        //cur_game.man.render(&cur_game.man_sprite_position);
        sprintf(score_display, "Score: %d", (int)score);
        sprintf(coin_display, "Coins:%d", (int)coin_collected);
        mfont.render_string(mrenderer, score_display, 10, 10);
        mfont.render_string(mrenderer, coin_display, 10, 30);
        if (isJumping)
        {
            //cur_game.man.render(&cur_game.man_sprite_position[27]);

            cur_game.man.render(&cur_game.man_sprite_position[(int)anim_frame]);
            //cur_game.man_sprite_position->x,cur_game.man_sprite_position->y+50}

        }
        else
        {
            if (anim_frame > 8)
                frame_increment = -1;
            else if (anim_frame < 0)
                frame_increment = 1;
            anim_frame += 0.1* frame_increment;
            cur_game.man.render(&cur_game.man_sprite_position[(int)anim_frame]);
        }
        //SDL_RenderDrawRectF(mrenderer, &cur_game.man.position_in_screen);
        //SDL_RenderDrawLine(mrenderer,colliderX,colliderY,SCREEN_HEIGHT,SCREEN_WIDTH);
        mrenderer.render();
        if (!isAlive)
        {
            menu_running = false;
            game_running = false;
        }

}
bgTexture.position_in_screen = { 0,0,SCREEN_HEIGHT,SCREEN_WIDTH };
after_death();
Mix_HaltMusic();
}
void Game::menu()
{
        bgTexture.position_in_screen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        while (menu_running)
        {
            mrenderer.clear(0, 0, 0, 255);
            bgTexture.render(nullptr);
            mfont.render_string(mrenderer, "START", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			//render high scores below start
			mfont.render_string(mrenderer, "HIGH SCORES", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50);
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
                        if (event.button.x > SCREEN_WIDTH / 2 - 50 && event.button.x < SCREEN_WIDTH / 2 + 100&&event.button.y > SCREEN_HEIGHT / 2 - 50 && event.button.y < SCREEN_HEIGHT / 2 + 50)
                        {
                            game_running = true;
                            menu_running = false;
                        }
						if (event.button.x > SCREEN_WIDTH / 2 - 50 && event.button.x < SCREEN_WIDTH / 2 + 100&&event.button.y> SCREEN_HEIGHT / 2 + 50 && event.button.y < SCREEN_HEIGHT / 2 + 100)
						{
							game_running = false;
							menu_running = false;
                            high_scores_display = true;
						}
                    }
					
                    break;
                }
                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        game_running = false;
						menu_running = false;
						break;
                    case SDLK_SPACE:
                    case SDLK_RETURN:
						game_running = true;
						menu_running = false;
						break;
					}
					break;
				}
                }
            }
            mrenderer.render();

        }
    }

void Game::load_game_assets()
{
	//lets allocate 1/total_assets for each png loaded
    cur_game.man_sprite_position[0].x = 150*0+50;
    cur_game.man_sprite_position[0].y = 25;
    cur_game.man_sprite_position[0].h = MAN_HEIGHT;
    cur_game.man_sprite_position[0].w = MAN_WIDTH;
    for (int i = 1; i < 28; i++)
    {
        cur_game.man_sprite_position[i].h = MAN_HEIGHT;
        cur_game.man_sprite_position[i].w = MAN_WIDTH;
        cur_game.man_sprite_position[i].x = 150*i+50;
        cur_game.man_sprite_position[i].y =cur_game.man_sprite_position[i-1].y ;
    }
        cur_game.man.load(mrenderer, "./misc/man.png");
		
        
    //cur_game.man_sprite_position = { (SCREEN_WIDTH / 2) - 75,(SCREEN_HEIGHT / 2) - 50,210,350 };
    cur_game.Coin.load(mrenderer, "./misc/coin.png");
    


	/*Now load all the obstacles in sprites array*/
    char data[100];
    for (int i = 1; i <NO_OF_OBSTACLES ; i++)
    {
        sprintf(data, "./misc/%d.png", i);
		cur_game.obstacles[i].load(mrenderer, (data));
        

    }
    cur_game.obstacles[16].load(mrenderer, "./misc/right_turn_ahead.png");
    cur_game.obstacles[17].load(mrenderer, "./misc/left_turn_ahead.png");
    cur_game.obstacles[18].load(mrenderer, "./misc/fire.png");
    
	/*................*/
	//these function write to global vector variable line
    generateLines(lines);
    generateObstacles((cur_game.obstacles),lines);
    generateCoins(cur_game.Coin,lines);
    return ;
}
void Game::load_screen()
{
    std::thread t1(&Game::load_game_assets, this);
    load_game_assets();
    SDL_FRect load_outline = { 20,500,700,50 };
    SDL_FRect load_fill = { 20,500,0,50 };
    while (load_percent <1)
    {
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				game_running = false;
				menu_running = false;
                goto EXIT;
				
			}
		}
        mrenderer.clear(0,0,0,255);
        mfont.render_string(mrenderer, "LOADING...", 20, 500 - 20);
        mrenderer.set_color(255, 255, 255, 255);
        SDL_RenderDrawRectF(mrenderer, &load_outline);
        mrenderer.set_color(0, 255, 0, 255);
        load_fill.w = load_percent * 700;
        SDL_RenderFillRectF(mrenderer, &load_fill);
        load_percent += 0.06;
        SDL_Delay(200);
        mrenderer.render();
    }
	
EXIT:
t1.join();
    return;
}

void Game::after_death()
{
    bgTexture.position_in_screen = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    read_high_scores_and_sort();
    high_scores_numbers[5] = score;
    sort(high_scores_numbers, 6);
	//write high scores to file
    std::fstream file("./bin/high_scores.bin",std::ios::binary|std::ios::out);
    if (!file.is_open())
    {
        std::cout << "Couldn't open file" << std::endl;
        return;
    }
    file.write(reinterpret_cast<char*>(&high_scores_numbers), sizeof(float) * 5);
    file.close();
	//while loop to wait for user to press space to restart
    while (!(game_running||menu_running))
    {
        mrenderer.clear(0,0,0,255);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                game_running = false;
                menu_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE)
                {
					game_running = true;
					menu_running = false;
					break;
                }
                else
				{
					game_running = false;
					menu_running = true;
					break;
				}
                break;
            }
        }
        bgTexture.render(nullptr);
        sprintf(score_display, "Score: %d", (int)score);
        sprintf(coin_display, "Coins:%d", (int)coin_collected);
		
        mfont.render_string(mrenderer, score_display, SCREEN_WIDTH/2, SCREEN_HEIGHT/2-50);
        mfont.render_string(mrenderer, coin_display, SCREEN_WIDTH/2, SCREEN_HEIGHT);
		mfont.render_string(mrenderer, "Press space to restart or any key to main menu", 10, SCREEN_HEIGHT - 20);
        mrenderer.render();
    }
    score = 0;
	coin_collected = 0;
}
void Game::high_scores()
{
    read_high_scores_and_sort();

	
	// write high_scores to file
	//while loop to wait for user to press space to restart
	while (high_scores_display)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				game_running = false;
				menu_running = false;
                high_scores_display = false;
				break;
			case SDL_KEYDOWN:
                if (event.key.keysym.sym)
                {
					high_scores_display = false;
                    game_running = false;
                    menu_running = true;
					break;
                }
				break;
			}
		}
		mrenderer.clear();
		bgTexture.render(nullptr);
        mfont.render_string(mrenderer, "High Scores", SCREEN_WIDTH / 2, 20);
		for (int i = 0; i < 5; i++)
		{
			sprintf(score_display, "%d. %d",i+1,(int) high_scores_numbers[i]);
			mfont.render_string(mrenderer, score_display, SCREEN_WIDTH / 2, 50 + i * 30);
		}
		mfont.render_string(mrenderer, "Press any key to main menu", 10, SCREEN_HEIGHT - 20);
		mrenderer.render();
	}
	score = 0;
	coin_collected = 0;
}
void sort(float* arr,int size)
{
    float temp = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = i; j < size; j++)
		{
			if (arr[i] < arr[j])
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
	
}
void Game::read_high_scores_and_sort()
{
    std::fstream file("./bin/high_scores.bin", std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        file.close();
        file.open("./bin/high_scores.bin", std::ios::out | std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            std::cout << "File couldn't be opened" << std::endl;
        }
        file.write(reinterpret_cast<char*>(&high_scores_numbers), sizeof(float) * 5);
    }
    file.read(reinterpret_cast<char*>(&high_scores_numbers), sizeof(float)*5);
    sort(high_scores_numbers, 5);
}