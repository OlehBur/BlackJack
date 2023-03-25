#pragma once

#include "GameItems.h"


class Chip : public GameItems {

	static unique_ptr <SDL_Texture, SDL_TxtrDeleter> chipTextrue;
	SDL_Rect positionR{ NULL }, 
		textureR{ NULL };
	bool isDealerChip{ false };

	bool InitTextures(SDL_Renderer* render);
	
public:

	Chip(const bool& isDealerChip, SDL_Renderer* render);
	//void Destructor_Chip();

	bool& IsDealerChip();

	void UpdateRect();
	void MoveToCoord(Coordinate x, Coordinate y);
	void Draw(SDL_Renderer* render);

	int& GetWidth();
};