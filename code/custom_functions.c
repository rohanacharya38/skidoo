#include "custom_functions.h"
SDL_Texture *load_img(char *image_path, SDL_Renderer *renderer)
{
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    uint8_t *data = stbi_load(image_path, &width, &height, &orig_format, req_format);
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

    SDL_Surface *surf = SDL_CreateRGBSurfaceFrom((void *)data, width, height, depth, pitch,
                                                 rmask, gmask, bmask, amask);

    SDL_Texture *ret = SDL_CreateTextureFromSurface(renderer, surf);
    return ret;
}

SDL_Rect _get_char_rect_cood(const char ch)
{
    SDL_Rect tmp = {0, 0, 14, 18};
    tmp.x = ((ch - 32) % 18) * tmp.w;
    tmp.y = ((ch - 32) / 18) * tmp.h;
    return tmp;
}
void render_string(const char *str, const int length, SDL_Texture *img_texture, SDL_Renderer *renderer)
{
    SDL_Rect char_rect;
    SDL_Rect d_rect = {.w = 14, .h = 18, .x = 0, .y = 0};
    for (int i = 0; i < length; i++)
    {
        if (str[i] == '\n')
        {
            d_rect.y += d_rect.h;
            d_rect.x = 0;
            continue;
        }
        char_rect = _get_char_rect_cood(str[i]);
        SDL_RenderCopy(renderer, img_texture, &char_rect, &d_rect);
        d_rect.x += d_rect.w;
    }
}
