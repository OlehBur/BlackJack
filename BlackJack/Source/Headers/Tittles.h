#pragma once

#include "GameItems.h"


/*identification keys for the main tittles of the game*/
#define TITTLE_YOUR_TURN		20
#define TITTLE_MADE_BET			21
#define TITTLE_MADE_BET1		22

#define TITTLE_TAKE_OR_SKIP		26
#define TITTLE_TAKE_OR_SKIP1	27

#define TITTLE_HELP				30
#define TITTLE_HELP_HIT			31
#define TITTLE_HELP_STAND		32
#define TITTLE_HELP_BET			33
#define TITTLE_HELP_YOUR_DECK	34

#define TITTLE_GAME_U_WIN		40
#define TITTLE_GAME_U_LOSE		41



class Tittle : public GameItems {
	SDL_Surface* textS{ nullptr },
		* outlineS{ nullptr };
	SDL_Texture* textT{ nullptr },
		* outlineT{ nullptr };
	TTF_Font* appFont{ nullptr },
		* outlineFont{ nullptr };
	SDL_Rect textR{ NULL };
	string text = "";

public:

	Tittle();
	Tittle(const string& str, SDL_Renderer* render,
		Coordinate x, Coordinate y,
		const int& sizeText,
		const string fontPath = "Resource\\TTF\\CONEI___.ttf");
	//~Tittle();
	void Destructor_Tittle();

	void UpdateRect();
	void InitFont(SDL_Renderer* render,
		Coordinate x, Coordinate y,
		const int& sizeText, 
		const string fontPath= "Resource\\TTF\\CONEI___.ttf");

	/*return false if tittle isnt Init*/
	bool SetText(const string& str, 
		SDL_Renderer* render, 
		const SDL_Color& fontColor = { 204, 102, 0, 255 },
		const SDL_Color& colorOutline = { 102,0,0, 255 });
	string& GetText();

	void Draw(SDL_Renderer* render);
};