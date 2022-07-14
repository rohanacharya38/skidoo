#pragma once
#include <iostream>
#include <SDL.h>

#define ASSERT_SDL(x) if (!(x)) { std::cout << "Assertion failed: " << #x << "ERROR: "<<SDL_GetError()\
                            <<"\nIn File: "<<__FILE__<<"In Line Number: "<<__LINE__<<std::endl;\
                            __debugbreak();\
                            SDL_Quit();\
                            }
                            
#define ASSERT(x) if (!(x)) { std::cout << "Assertion failed: " << #x <<"\nIn File: "<<__FILE__<<"In Line Number: "<<__LINE__<<std::endl;\
                            __debugbreak();\
                               }


class string {
private:
    char* data;
public:
    int length;
    string(const char* data,int length)
    {
        this->data = new char[length];
        if (this->data != nullptr)
        {

            for (int i = 0; i < length; i++)
            {
                this->data[i] = data[i];
            }
            
        }
        this->length = length;

    }
    
    char operator [](int i)
    {
        return data[i];
    }
    operator char*()
    {
        return data;
    }
    

};