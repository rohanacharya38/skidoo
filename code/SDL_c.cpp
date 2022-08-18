#ifdef __linux__ 
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL2\SDL.h>
#endif
#include "headers/SDL_c.h"
#include "headers/defines.h"
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
window::window(const char *title, int w, int h)
{

    mwindow = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    ASSERT_SDL(mwindow);

}

window::~window()
{

    SDL_DestroyWindow(mwindow);
}

renderer::renderer(window &w, int index)
{
    mrenderer = SDL_CreateRenderer(w.mwindow, index, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ASSERT_SDL(mrenderer);
}
SDL_Rect font::_get_char_rect_cood(const char ch)
{
    SDL_Rect tmp = {0, 0, 14, 18};
    tmp.x = ((ch - 32) % 18) * tmp.w;
    tmp.y = ((ch - 32) / 18) * tmp.h;
    return tmp;
}


char * font::load_file(renderer &r, const char *path)
{
    mfont = stb::load_img(path, r,nullptr);
    if (mfont == nullptr)
    {

    }
    else
    {
        return nullptr;
    }
}
font::font(renderer &r,const char *path)
{
    load_file(r,path);
}
SDL_Texture* stb::load_img(const char* image_path, SDL_Renderer* renderer,SDL_FRect *texture_rect)
{
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    uint8_t* data = stbi_load(image_path, &width, &height, &orig_format, req_format);

    if (!data)
    {
        std::string error = stbi_failure_reason();
        return nullptr;
    }
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    int depth, pitch;
    depth = 32;
    pitch = 4 * width;

    SDL_Surface *surf = SDL_CreateRGBSurfaceFrom((void *)data, width, height, depth, pitch,rmask, gmask, bmask, amask);
    if (texture_rect != nullptr)
    {
    texture_rect->h = height;
    texture_rect->w = width;
    }
    if (surf == nullptr)
        return nullptr;
    SDL_Texture *ret = SDL_CreateTextureFromSurface(renderer, surf);
    if (ret == nullptr)
        return nullptr;
    else
    return ret;
}
    void renderer::clear()
    {
        SDL_RenderClear(mrenderer);
    }
    void renderer::clear(uint8_t r, uint8_t g, uint8_t b,uint8_t a)
    {
		SDL_SetRenderDrawColor(mrenderer, r, g, b, a);
		SDL_RenderClear(mrenderer);
    }
    void renderer::render()
    {
        SDL_RenderPresent(mrenderer);
    }
    void renderer::set_color(int r, int g, int b, int a)
    {
        SDL_SetRenderDrawColor(mrenderer, r, g, b, a);
    }
    renderer::~renderer()
    {
		SDL_DestroyRenderer(mrenderer);
    }
    font::~font()
    {
        SDL_DestroyTexture(mfont);
    }
    void renderer::loadTexture(Texture& t, SDL_Rect* srcRect, SDL_Rect* destRect)
    {
        if (SDL_RenderCopy(mrenderer, t, srcRect, destRect) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
        }
    }