#pragma once
#ifdef __linux__ 
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL.h>
#endif
#include "defines.h"

class window;
class renderer;
class Texture;
class font;
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
    friend class Texture;
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
    void loadTexture(Texture& t, SDL_Rect* srcRect = nullptr, SDL_Rect* destRect = nullptr);
	
    ~renderer();
};

class font
{
private:
    SDL_Texture *mfont;
    SDL_Rect _get_char_rect_cood(const char ch);
public:
    void render_string(SDL_Renderer* renderer,std::string str, int xPos = 0, int yPos = 0)
    {
        SDL_Rect char_rect;
        SDL_Rect d_rect = { xPos, yPos,14, 18 };
        for (size_t i = 0; i <str.length(); i++)
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
class Texture
{
protected:
	SDL_Texture *mtexture;
	SDL_Renderer *mrenderer;

	friend class renderer;
	public:
        Texture(renderer& r,const char* path)
        {
			mtexture=stb::load_img(path,r);
            mrenderer = r;
        }
		Texture()
		{
			mtexture = nullptr;
            mrenderer = nullptr;
		}
		
		operator SDL_Texture* ()
		{
			return mtexture;
		}
        void  render()
        {
			SDL_RenderCopy(mrenderer, mtexture, nullptr, nullptr);
        }
        void load(renderer& r, const char* path)
        {
            mtexture = stb::load_img(path, r);
            mrenderer = r;
        }
        
        ~Texture()
        {
			SDL_DestroyTexture(mtexture);
        }
};
class Sprite :public Texture
{
private:

    SDL_Renderer* mrenderer;

public:
    SDL_Rect position_in_screen;
    Sprite(renderer& r, const char* path):Texture(r, path), mrenderer((SDL_Renderer*)r)
    {
        position_in_screen = { 0 };
       
    }
    Sprite(): mrenderer(nullptr)
    {
        position_in_screen = { 0 };
    }
    void render(SDL_Rect *position_in_spritesheet,SDL_Rect *position_in_screen)
    {
        SDL_RenderCopy(mrenderer, mtexture, position_in_spritesheet, position_in_screen);
    }
    void load(renderer& r, const char* path)
    {
        mrenderer = r;
        mtexture = stb::load_img(path, r);
    }

    
};