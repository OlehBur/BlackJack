#include "Headers/Button.h"
#include "Headers/GameItems.h"

Button::Button(/*const SDL_Rect& position,*/ SDL_Renderer* render, const string& tittle) {
	//this->render = render;
	this->tittle = tittle;
	texture = GameItems::LoadTexture("Resource\\Images\\Cards skins\\Default\\jack_of_clubs2.png"/*"Resource\\Images\\GUI\\buttons.png"*/, render);

	SDL_QueryTexture(texture, NULL, NULL, &textureR.w, &textureR.h);
	buttonTextureW = textureR.w;
	buttonTextureH = textureR.h;

	textureR.y = textureR.x = 0;
	textureR.h = buttonTextureH /3;
	textureR.w = buttonTextureW;

	positionR.x = 0;// position.x;
	positionR.y = 0;// position.y;
	positionR.h = textureR.h /2;
	positionR.w = buttonTextureW /2;
};

Button::~Button() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	/*SDL_DestroyRenderer(render);
	render = nullptr;*/
	
};

SDL_Rect& Button::GetRect() {
	return positionR;
};

void Button::SetCoord(const SDL_Rect& position) {
	positionR.x = position.x;
	positionR.y = position.y;
};

#include <iostream>
void Button::Update(const SDL_Rect& mousePos, const bool& isClick) {
	if (SDL_HasIntersection(&positionR, &mousePos)) {
		isSelected = true;
		textureR.y = 0;
		if (isClick)
			textureR.y = (buttonTextureH / 3) * 2;
		std::printf("TT: %s, X: %d, Y: %d.\n", &tittle, mousePos.x, mousePos.y);
	}
	else {
		isSelected = false;
		textureR.y = buttonTextureH/3;
	}
};

void Button::Draw(SDL_Renderer* render) {
	SDL_RenderCopy(render, texture, &textureR, &positionR);
	//GameItems::DrawText(render, 300, 300, tittle);
};