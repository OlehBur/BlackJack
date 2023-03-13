#pragma once

#include "GameItems.h"

class Chip : public GameItems {
	//SDL_Color chipColor{255,0,0}
	SDL_Texture* chipTextrue{ nullptr };
	SDL_Rect positionR{ NULL }, 
		textureR{ NULL };
	//Point center;
	bool isDealerChip{ false };
	bool InitTextures(SDL_Renderer* render);
	
public:

	Chip() {};
	Chip(const bool& isDealerChip/*, Coordinate x, Coordinate y*/, SDL_Renderer* render);
	void Destructor_Chip();

	bool& IsDealerChip();

	void UpdateRect();
	void MoveToCoord(Coordinate x, Coordinate y);
	void Draw(SDL_Renderer* render);

	int& GetWidth();
	//bool operator<(Chip& ñhip);
	//bool operator>(Chip& ñhip);
	//void operator=(Chip ñhip);
};