#pragma once
#ifndef LINE_H
#define LINE_H
#include<cmath>
#include <SDL.h>
#include "SDL_c.h"

//-----CLASS TO MANIPULATE LINES------//

constexpr const int height = 768;
constexpr const int width = 1024;

constexpr const float camD = 0.84; //camera depth
constexpr const int roadW = 2000; //width of road in 3D
constexpr const int segL = 200; //width of each segment of road

class Line
{
private:
    float x, y, z; //real world coordinate in 3d
    float X, Y, W; //screen coordinates
    float curve; // for curve in road
    float scale;
    float spriteX; // X distance of obstacle from road
    float coinX;
    Sprite sprite; //sprite to render obstacle
	Sprite coin; //sprite to render coin    
    SDL_FRect texture_rect; 
    SDL_FRect coin_texture;

public:
    Line()
    {
        coinX = spriteX = curve = x = y = z = 0;
    }

    ~Line() {}

    //////////////////////////////////////////////////////////////////////////////////
    ////      FUNCTION TO PROJECT 3D LINES TO SCREEN BY 2D CONVETSION           /////
    //// Here the CAMERAS indicate FROM WHAT POSITION THE POINT IS BEING VIEWED/////
    /////                                                                    //////
    ////-------------CONVERSION BASED ON SIMILAR TRIANGLES--------------------////
    /*      #First the point is TRANSLATED to CAMERA COORDINATE                */
    /*      #Then PROJECTED                                                   */
    /*      #Finally SCALED DOWN to SCREEN COORDINATE                        */
    //////////////////////////////////////////////////////////////////////////

    void projectToScreen(int camX, int camY, int camZ)
    {
        scale = camD / (z - camZ);
        X = (1 + scale * (x - camX)) * width / 2;
        Y = (1 - scale * (y - camY)) * height / 2;
        W = scale * roadW * width / 2;
    }

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
        s.render(nullptr);      //TODO:Check wethher sprite is scaled or not
    }

    SDL_FRect getObstacleBounds()
    {
        return texture_rect;
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
		coin_texture = s.position_in_screen;

        s.render(nullptr);
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

    friend void generateLines();
    friend void generateObstacles(Sprite[]);
    friend void generateCoins(Sprite&);
};


#endif // LINE_H