#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "GameItems.h"
#include "Tittles.h"

enum class ClickedButton { HIT, STAND, CASH_PLUS, CASH_MINUS, CASH, HELP, CHANGE_SKIN, NOTHING/* DEFAULT, DEFAULT_MINI */ };

#define BUTTON_TYPE_HIT				ClickedButton::HIT
#define BUTTON_TYPE_STAND			ClickedButton::STAND
#define BUTTON_TYPE_CASH_PLUS		ClickedButton::CASH_PLUS
#define BUTTON_TYPE_CASH_MINUS		ClickedButton::CASH_MINUS
#define BUTTON_TYPE_CASH			ClickedButton::CASH
#define BUTTON_TYPE_HELP			ClickedButton::HELP
#define BUTTON_TYPE_CHANGE_SKIN		ClickedButton::CHANGE_SKIN
#define BUTTON_TYPE_NOTHING			ClickedButton::NOTHING


#define Button_Click_Texture_H(heightTexture) (heightTexture/3)*2
#define Button_Default_Texture_H(heightTexture) (heightTexture/3)



class Button {
	ClickedButton type;
	unique_ptr <SDL_Texture, SDL_TxtrDeleter> texture;
	SDL_Rect textureR{ NULL }, positionR{ NULL };
	bool isSelected{ false }, 
		isClicked{ false };
	/*string tittle{ "" };*/
	int buttonTextureW{ 0 },
		buttonTextureH{ 0 };
	float scale{ 0.5f };
	Tittle tittle;

	//void UpdateRect();
	void InitTexture();

public:
	static ClickedButton currentButtonClicked;

	/*
	* isPushButton - parameter indicating whether the button can be pressed
	type - the type of the key identifies it, for definitions use: 
	BUTTON_TYPE_HIT
	BUTTON_TYPE_STAND
	BUTTON_TYPE_CASH_PLUS 
	BUTTON_TYPE_CASH_MINUS
	BUTTON_TYPE_NOTHING
	*/
	Button(SDL_Renderer* render, const ClickedButton& type/*, const string& tittle*/, const int& sizeText/*=60*/);
	//~Button();
	//void Destructor_Button();

	/*static ClickedButton& GetClickedButton();*/
	SDL_Rect& GetRect();
	string& GetTittle();
	void SetCoord(Coordinate x, Coordinate y);
	void SetTittle(const string& tittle, SDL_Renderer* render);

	/*
	Button reactions to mouse events
	(outputs TRUE if the mouse clicked on the button, otherwise FALSE)
	*/
	bool Interact(const SDL_Rect& mousePos, const bool& isClick);;
	void Draw(SDL_Renderer* render);
};

