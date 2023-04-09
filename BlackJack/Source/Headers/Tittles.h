#pragma once

#include "GameItems.h"


/*identification keys for the main tittles of the game*/
#define TITTLE_YOUR_TURN		20
#define TITTLE_MADE_BET			21
#define TITTLE_MADE_BET1		22

#define TITTLE_TAKE_OR_SKIP		26
#define TITTLE_TAKE_OR_SKIP1	27

#define TITTLE_HELP				30
#define TITTLE_HELP1			31
#define TITTLE_HELP2			32
#define TITTLE_HELP3			33
#define TITTLE_HELP4			34
#define TITTLE_HELP5			35
#define TITTLE_HELP6			36
#define TITTLE_HELP7			37

#define TITTLE_GAME_END_ROUND	40
#define TITTLE_GAME_END			41


constexpr SDL_Color tittle_text_color = { 204, 102, 0, 255 };
constexpr SDL_Color tittle_outline_color = { 102, 0, 0, 255 };
constexpr int tittle_default_text_size = 30;
constexpr int tittle_minimal_outline_size = 1;
 
class Tittle : public GameItems {
	unique_ptr <SDL_Surface, SDL_SrfcDeleter> textS,
		outlineS;
	unique_ptr <SDL_Texture, SDL_TxtrDeleter> textT,
		outlineT;
	unique_ptr <TTF_Font, SDL_FontDeleter> appFont,
		outlineFont;
	SDL_Rect textR{ NULL };
	string text = "";

public:

	Tittle();
	Tittle(const string& str, SDL_Renderer* render,
		Coordinate x, Coordinate y,
		const int& sizeText,
		const string fontPath = "Resource\\TTF\\CONEI___.ttf");
	/*~Tittle();*/

	//void Destructor_Tittle();

	void UpdateRect();
	void InitFont(SDL_Renderer* render,
		Coordinate x, Coordinate y,
		const int& sizeText, 
		const string fontPath= "Resource\\TTF\\CONEI___.ttf");

	/*return false if tittle isnt Init*/
	bool SetText(const string& str, 
		SDL_Renderer* render, 
		const SDL_Color& fontColor = tittle_text_color,
		const SDL_Color& colorOutline = tittle_outline_color);
	string& GetText();

	void Draw(SDL_Renderer* render);
};