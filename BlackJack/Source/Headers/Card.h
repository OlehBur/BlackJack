#pragma once

#include "GameItems.h"


/*
placement on the screen affects the angle of rotation of the opponent's cards relative to the player,
	can be: 
	- CARD_PLACE_DEFAULT,
	- CARD_PLACE_RIGHT,
	- CARD_PLACE_LEFT
*/
typedef const int& ScreenPlacement;

/*
Card suit type (integers from 10 to 13), can be:
	- CARD_SUIT_SPADE,
	- CARD_SUIT_CLUB,
	- CARD_SUIT_HEART,
	- CARD_SUIT_DIAMOND
*/
typedef const int& SuitType;

/*
Card type (integers from 102 to 114), can be:
	CARD_TYPE_TWO
	CARD_TYPE_THREE
	CARD_TYPE_FOUR
	CARD_TYPE_FIVE
	CARD_TYPE_SIX
	CARD_TYPE_SEVEN
	CARD_TYPE_EIGHT
	CARD_TYPE_NINE
	CARD_TYPE_TEN
	CARD_TYPE_JACK
	CARD_TYPE_QUEEN
	CARD_TYPE_KING
	CARD_TYPE_ACE
*/
typedef const int& CardType;

	//Placement Types
#define CARD_PLACE_DEFAULT 0
#define CARD_PLACE_RIGHT 1
#define CARD_PLACE_LEFT 2

	//Suit Types
#define CARD_SUIT_SPADE		10 //перев серц
#define CARD_SUIT_CLUB		11 //листочок
#define CARD_SUIT_HEART		12 //серце
#define CARD_SUIT_DIAMOND	13 //ромб

	//Card Types
#define CARD_TYPE_TWO	102
#define CARD_TYPE_THREE	103
#define CARD_TYPE_FOUR	104
#define CARD_TYPE_FIVE	105
#define CARD_TYPE_SIX	106
#define CARD_TYPE_SEVEN	107
#define CARD_TYPE_EIGHT	108
#define CARD_TYPE_NINE	109
#define CARD_TYPE_TEN	110
#define CARD_TYPE_JACK	111
#define CARD_TYPE_QUEEN	112
#define CARD_TYPE_KING	113
#define CARD_TYPE_ACE	114

class Card : public GameItems {
	int suit{ 0 }, type{ 0 };
	float scale{ 0.5f };
	SDL_Texture* topTexture{ nullptr },
		* suitTexture{ nullptr };
	bool isUpsideDown = true;
	SDL_Rect positionR{ NULL };
	float angleRotaton = 0.0f;
	const int cardMovingSpeed = 100;

	void UpdateRect();

public:
	static string skin;

	Card(SuitType suit, CardType card, SDL_Renderer* render, ScreenPlacement sp = CARD_PLACE_DEFAULT, Coordinate x = 0, Coordinate y = 0);
	void /*~Card*/Destructor_Card();

	void InitTexture(SDL_Renderer* render);
	/*
	return true if card coordinates have reached end point 
	*/
	bool AnimateMotion(const Point& p1, const float& deltatime, SDL_Renderer* render, bool isNormalPlacement);
	void Draw(SDL_Renderer* render);
	//static void DrawDeck(SDL_Renderer* render, int x, int y);

	void SetScaleTexture(const float& scale);
	void SetScaleTextureByScreen(const int& screenWidth);
	void SetCardPos(Coordinate x, Coordinate y);
	void SetPlacement(ScreenPlacement sp);
	void SetUpsideDown(const bool& isUpsideDown);
	static void SetCardSkin(const string& name);

	/*SuitType*/int& GetSuit();
	/*CardType*/int& GetType();
	/*
	return value of cards, 
	(number cards return corresponding values, face cards return 10, and ace returns -1)
	*/
	int GetValue();
	SDL_Rect& GetRectOnScreen();


	bool operator<(Card& card);
	bool operator>(Card& card);
	void/*Card&*/ operator=(Card card);
};