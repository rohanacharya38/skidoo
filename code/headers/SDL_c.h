#pragma once
#ifdef __linux__ 
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL.h>
#endif
#include "defines.h"
class window
{
private:
    SDL_Window* mwindow;
    friend class renderer;
public:
    window(const char* title, int w, int h);
    
    ~window();
};

class renderer
{
private:
    SDL_Renderer* mrenderer;
public:
    renderer(window& w, int index);
    renderer()
    {
        mrenderer=nullptr;
    }
    void clear();
    void render();
    void set_color(int r=0, int g=0, int b=0, int a=255);
    operator SDL_Renderer* ()
    {
        return mrenderer;
    }
    ~renderer();
};

class font
{
private:
    SDL_Texture *mfont;
    SDL_Rect _get_char_rect_cood(const char ch);
public:
    void render_string(SDL_Renderer* renderer, string str, int xPos = 0, int yPos = 0)
    {
        SDL_Rect char_rect;
        SDL_Rect d_rect = { xPos, yPos,14, 18 };
        for (int i = 0; i <str.length; i++)
        {
            if (str[i] == '\n')
            {
                d_rect.y += d_rect.h;
                d_rect.x = xPos;
                continue;
            }
            char_rect = _get_char_rect_cood(str[i]);
            SDL_RenderCopy(renderer, mfont, &char_rect, &d_rect);
            d_rect.x += d_rect.w;
        }
}

    font(renderer& r, const char* path);
    font()
    {
        mfont=nullptr;
    }
    char * load_file(renderer &r,const char*path);
    ~font();
};
namespace stb
{
SDL_Texture *load_img(const char *image_path, SDL_Renderer *renderer);
}