#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "SDL_mouse.h"
#include "GameItems.h"


enum class ClickedButton{ HIT, STAND, CASH_PLUS, CASH_MINUS, NOTHING};

#define BUTTON_TYPE_HIT			ClickedButton::HIT
#define BUTTON_TYPE_STAND		ClickedButton::STAND
#define BUTTON_TYPE_CASH_PLUS	ClickedButton::CASH_PLUS
#define BUTTON_TYPE_CASH_MINUS	ClickedButton::CASH_MINUS
#define BUTTON_TYPE_NOTHING		ClickedButton::NOTHING


class Button {
	ClickedButton type;
	SDL_Texture* texture{ nullptr };
	SDL_Rect textureR{ NULL }, positionR{ NULL };
	bool isSelected{ false }, 
		isClicked{ false };
	string tittle{ "" };
	int buttonTextureW{ 0 },
		buttonTextureH{ 0 };
	float scale{ 0.5f };

	//void UpdateRect();
	void InitTexture();

public:
	static ClickedButton currentButtonClicked;

	/*
	the type of the key identifies it, for definitions use: 
	BUTTON_TYPE_HIT
	BUTTON_TYPE_STAND
	BUTTON_TYPE_CASH_PLUS 
	BUTTON_TYPE_CASH_MINUS
	BUTTON_TYPE_NOTHING
	*/
	Button(SDL_Renderer* render, const ClickedButton& type, const string& tittle);
	//~Button();
	void Destructor_Button();

	/*static ClickedButton& GetClickedButton();*/
	SDL_Rect& GetRect();
	string& GetTittle();
	void SetCoord(Coordinate x, Coordinate y);
	void SetTittle(const string& tittle);

	/*
	Button reactions to mouse events
	(outputs TRUE if the mouse clicked on the button, otherwise FALSE)
	*/
	bool Interact(const SDL_Rect& mousePos, const bool& isClick);
	void Draw(SDL_Renderer* render);
};

