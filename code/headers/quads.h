#pragma once
#include <SDL2/SDL.h>
#include "SDL_classes.h"
class Quad
{
private:
	SDL_Vertex vertices1[3];
	SDL_Vertex vertices2[3];
	bool has_texture;
	SDL_Texture* texture;

public:
	Quad( )
	{
		has_texture = false;
	}
	Quad(SDL_Texture* texture)
	{
		has_texture = true;
		this->texture = texture;
	}
	void render(SDL_Renderer* renderer, SDL_Color quadColor, float x1, float y1, float w1, float x2, float y2, float w2)
	{
		vertices1[0] = { SDL_FPoint{ x1 - w1 , y1 }, quadColor, SDL_FPoint{ 0 } };
		vertices1[1] = { SDL_FPoint{ x2 - w2 , y2 }, quadColor, SDL_FPoint{ 0 } };
		vertices1[2] = { SDL_FPoint{ x1 + w1 , y1 }, quadColor, SDL_FPoint{ 0 } };



		vertices2[0] = vertices1[2];
		vertices2[1] = vertices1[1];
		vertices2[2] = { SDL_FPoint{ x2 + w2, y2 }, quadColor, SDL_FPoint{ 0 } };
		if (!has_texture)
		{
			SDL_RenderGeometry(renderer, nullptr, vertices1, 3, nullptr, 0);
			SDL_RenderGeometry(renderer, nullptr, vertices2, 3, nullptr, 0);
		}
		else
			{
			SDL_RenderGeometry(renderer, texture, vertices1, 3, nullptr, 0);
			SDL_RenderGeometry(renderer, texture, vertices2, 3, nullptr, 0);
		}
	}
	~Quad() {}
};
