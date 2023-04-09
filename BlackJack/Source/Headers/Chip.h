#pragma once

#include "GameItems.h"


constexpr float chip_texture_scale = 1 / 2.0f;


class Chip : public GameItems {

	/*unique_ptr <SDL_Texture, SDL_TxtrDeleter>*/SDL_Texture* chipTextrue;
	SDL_Rect positionR{ NULL }, 
		textureR{ NULL };
	bool isDealerChip{ false };

	bool InitTextures(SDL_Renderer* render);
	
public:

	//Chip(const Chip& chip);
	Chip(const bool& isDealerChip, SDL_Renderer* render);
	//void Destructor_Chip();

	bool& IsDealerChip();

	void UpdateRect();
	void MoveToCoord(Coordinate x, Coordinate y);
	void Draw(SDL_Renderer* render);

	int& GetWidth();
};