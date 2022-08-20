#pragma once
#ifndef LINE_H
#define LINE_H
#include<cmath>
#include <SDL2/SDL.h>
#include "SDL_classes.h"
#include <vector>
//-----CLASS TO MANIPULATE LINES------//

constexpr const int height = 768;
constexpr const int width = 1024;

constexpr const float camD = 0.84; //camera depth
constexpr const int roadW = 2000; //width of road in 3D
constexpr const int segL = 200; //width of each segment of road

class Line
{
private:
    float x, y, z;
    float X, Y, W; //screen coordinates
    float curve; // for curve in road
    float scale;
    float spriteX; // X distance of obstacle from road
    float coinX;
    Sprite sprite; //sprite to render obstacle
	Sprite coin; //sprite to render coin    

public:
    SDL_FRect texture_rect; 
    bool collected;
    SDL_FRect coin_texture;
    Line()
    {
        coinX = spriteX = curve = x = y = z = 0;
        collected = false;
    }
	

    ~Line() {}
    SDL_Texture* get_sprite_texture()
    {
        return sprite;
    }


    void projectToScreen(int camX, int camY, int camZ)
    {
		/* 
        float Xcamera = x - camX;
		float Ycamera = y - camY;
		float Zcamera = z - camZ;
		

		
        scale = camD / Zcamera;
		float Xprojected=Xcamera* scale;
		float Yprojected=Ycamera* scale;
		
        X = (1 + Xprojected) * width / 2;
		Y = (1 - Yprojected) * height / 2;
        W = scale * roadW * width / 2;*/
		/*This shortened to code below*/
        scale = camD / (z - camZ);
        X = (1 + scale * (x - camX)) * width / 2;
        Y = (1 - scale * (y - 1500)) * height / 2;
        W = scale * roadW * width / 2;
    }
//camD=camera depth=distance from camera to road=0.84 always constant
//camZ=z coordinate of point from camera
//camY=
	
    //---------TO DRAW OBSTACLES---------//
    void drawSprite(SDL_Renderer * renderer)
    {
        Sprite s = sprite;
		
        int w = s.position_in_screen.w;
        int h = s.position_in_screen.h;
        float destX = X + scale * spriteX * width / 2;
        float destY = Y;
        float destW = w * W / 266;
        float destH = h * W / 266;

        destX += destW * spriteX;
        destY += destH * (-1);

        s.position_in_screen={ 0, 0,(float) w, h - (h / destH) };
        s.setScale(destW / w, destH / h);
        s.setPosition(destX, destY);
        s.position_in_screen = s.getGlobalBounds();
        texture_rect = s.position_in_screen;
        //SDL_RenderDrawRectF(renderer, &texture_rect);
		
        s.render(nullptr);      
    }

    SDL_FRect getObstacleBounds()
    {
        return texture_rect;
    }
    SDL_FRect *getObstacleBoundsPtr()
    {
        return &texture_rect;
    }

    //---------TO DRAW COINS--------------//
    void drawCoin(SDL_Renderer *renderer)
    {
        Sprite s = coin;
        float w = s.position_in_screen.w;
        float h = s.position_in_screen.h;

        float destX = X + scale * coinX * width / 2;
        float destY = Y - 2;
        float destW = w * W / 266;
        float destH = h * W / 266;

        destX += destW * coinX;
        destY += destH * (-1);

        s.position_in_screen = { 0, 0, w, h - h / destH };
        s.setScale(destW / w, destH / h);
        s.setPosition(destX, destY);

        s.position_in_screen = s.getGlobalBounds();
		coin_texture = s.getGlobalBounds();
        if (coin_texture.w > 0 && coin_texture.h > 0)
        {
        //SDL_RenderDrawRectF(renderer, &coin_texture);
		if(!collected)
        s.render(nullptr);
        }
    }


    SDL_FRect getCoinBounds()
    {
        return coin_texture;
    }

    //----------------------------------------//
    float gety() { return y; }
    float getX() { return X; }
    float getY() { return Y; }
    float getW() { return W; }
    float getCurve() { return curve; }
    //----------------------------------------//

    friend void generateLines(std::vector<Line>& lines);
    friend void generateObstacles(Sprite[]  , std::vector<Line>& lines);
    friend void generateCoins(Sprite&, std::vector<Line>& lines);
};


#endif // LINE_H
