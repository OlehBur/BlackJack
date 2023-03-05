#pragma once

#include "GameItems.h"

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

class Card /*: public GameItems*/ {
	int suit{ 0 };
	int type{ 0 };

public:
	//Card();
	Card(SuitType suit, CardType card);
	~Card() {};

	void AnimateMotion();
	//void Draw();

	/*SuitType*/int& GetSuit();
	/*CardType*/int& GetType();

	bool operator<(Card& card);
	bool operator>(Card& card);
	Card operator=(Card card);
};