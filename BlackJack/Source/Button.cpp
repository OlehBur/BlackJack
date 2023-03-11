#include "Headers/Button.h"
#include "Headers/GameItems.h"

ClickedButton Button::currentButtonClicked = ClickedButton::NOTHING;

void Button::InitTexture() {
	SDL_QueryTexture(texture,
		NULL, NULL,
		&textureR.w, &textureR.h);

	buttonTextureW = textureR.w;
	buttonTextureH = textureR.h;

	textureR.y = textureR.x = 0;
	textureR.h = buttonTextureH / 3;
	textureR.w = buttonTextureW;

	positionR.x = 0;
	positionR.y = 0;
	positionR.h = textureR.h * scale;
	positionR.w = buttonTextureW * scale;
};

Button::Button(SDL_Renderer* render, const ClickedButton& type, const string& tittle) {
	this->tittle = tittle; 
	texture = GameItems::LoadTexture("Resource\\Images\\GUI\\buttons.png", render);
	this->type = type;

	InitTexture();
};

void Button::Destructor_Button/*~Button*/() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	
};

SDL_Rect& Button::GetRect() {
	return positionR;
};

string& Button::GetTittle() {
	return tittle;
};


void Button::SetCoord(Coordinate x, Coordinate y) {
	positionR.x = x;
	positionR.y = y;
};

void Button::SetTittle(const string& tittle) {
	this->tittle = tittle;
};

bool Button::Interact(const SDL_Rect& mousePos, const bool& isClick) {
	if (SDL_HasIntersection(&positionR, &mousePos)) {
		isSelected = true;
		textureR.y = 0;
		if (isClick) {
			isClicked = true;
			textureR.y = (buttonTextureH / 3) * 2;
			currentButtonClicked = type;
			return true;
		}
		isClicked = false;
	}
	else {
		isSelected = false;
		textureR.y = buttonTextureH/3;
	}
	return false;
};

//void Button::UpdateRect() {
//	textureR.y = (isClicked) ?
//		(buttonTextureH / 3) * 2 : (isSelected)?
//		0 : buttonTextureH / 3;
//};

void Button::Draw(SDL_Renderer* render) {
	//UpdateRect();
	//SDL_RenderDrawRect(render, &textureR);
	//SDL_RenderDrawRect(render, &positionR);
	SDL_RenderCopy(render, 
		texture, 
		&textureR, 
		&positionR);
	//GameItems::DrawText(render, 300, 300, tittle);
};