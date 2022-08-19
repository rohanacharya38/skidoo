#pragma once
#include <iostream>
#ifdef __linux__ 
    #include <SDL2/SDL.h>
    void __debugbreak();
#define sprintf_s sprintf
#elif _WIN32
    #include <SDL2/SDL.h>
#endif

#define ASSERT_SDL(x) if (!(x)) { std::cout << "Assertion failed: " << #x << "ERROR: "<<SDL_GetError()\
                            <<"\nIn File: "<<__FILE__<<"In Line Number: "<<__LINE__<<std::endl;\
                            __debugbreak();\
                            SDL_Quit();\
                            }
                            
#define ASSERT(x) if (!(x)) { std::cout << "Assertion failed: " << #x <<"\nIn File: "<<__FILE__<<"In Line Number: "<<__LINE__<<std::endl;\
                            __debugbreak();\
                               }

constexpr const int SCREEN_WIDTH = 1024;
constexpr const int SCREEN_HEIGHT = 768;
constexpr const int MAN_HEIGHT = 115;
constexpr const int MAN_WIDTH = 55;
constexpr const int GAP_BETWEEN_2_SPRITE = 50;

