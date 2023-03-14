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

Button::Button(SDL_Renderer* render, const ClickedButton& type,/* const string& tittle,*/ const int& sizeText) {
	this->tittle.InitFont(render, 0, 0, sizeText/*const string fontPath*/);
	this->type = type;

	if (type == BUTTON_TYPE_HIT || type == BUTTON_TYPE_STAND || type == BUTTON_TYPE_CASH || type == BUTTON_TYPE_CHANGE_SKIN)
		texture = GameItems::LoadTexture("Resource\\Images\\GUI\\buttons.png", render);
	else //CASH_PLUS, CASH_MINUS, DEFAULT_MINI
		texture = GameItems::LoadTexture("Resource\\Images\\GUI\\buttons1.png", render);

	InitTexture();
};

void Button::Destructor_Button/*~Button*/() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	tittle.Destructor_Tittle();// ~Tittle();
};

SDL_Rect& Button::GetRect() {
	return positionR;
};

string& Button::GetTittle() {
	return tittle.GetText();
};


void Button::SetCoord(Coordinate x, Coordinate y) {
	positionR.x = x;
	positionR.y = y;

	tittle.MoveToCoord(x + positionR.w / 2, y + positionR.h / 2);
};

void Button::SetTittle(const string& tittle, SDL_Renderer * render) {
	this->tittle.SetText(tittle, render);
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

void Button::Draw(SDL_Renderer* render) {

	SDL_RenderCopy(render, 
		texture, 
		&textureR, 
		&positionR);
	tittle.Draw(render);
};

//bool Button::operator==(const ClickedButton& type) const {
//	return this->type == type;
//}