#pragma once

#include "GameItems.h"

class Chip : public GameItems {
	//SDL_Color chipColor{255,0,0}
	SDL_Texture* chipTextrue{ nullptr };
	SDL_Rect positionR{ NULL };
	Point center;
	bool isDealerChip{ false };
	bool InitTextures(SDL_Renderer* render);
	
public:

	Chip(const bool& isDealerChip, Coordinate x, Coordinate Y, SDL_Renderer* render);
	~Chip();

	bool& IsDealerChip();

	void UpdateRect();
	void Draw(SDL_Renderer* render);
};