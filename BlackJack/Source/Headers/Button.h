#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "SDL_mouse.h"
#include "GameItems.h"




class Button {
	SDL_Texture* texture{ nullptr };
	//SDL_Renderer* render{ nullptr };
	SDL_Rect textureR{ NULL }, positionR{ NULL };
	bool isSelected{ false };
	string tittle{ "" };
	int buttonTextureW{ 0 },
		buttonTextureH{ 0 };

public:
	
	//Button() {};
	Button(/*const SDL_Rect& position,*/ SDL_Renderer* render, const string& tittle);
	~Button();

	SDL_Rect& GetRect();

	void SetCoord(const SDL_Rect& position);
	void Update(const SDL_Rect& mousePos, const bool& isClick);
	void Draw(SDL_Renderer* render);
};

