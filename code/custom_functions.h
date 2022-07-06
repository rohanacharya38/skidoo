#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
SDL_Texture *load_img(char *image_path, SDL_Renderer *renderer);
void render_string(const char *str, const int length, SDL_Texture *img_texture, SDL_Renderer *renderer);
SDL_Rect _get_char_rect_cood(const char ch);