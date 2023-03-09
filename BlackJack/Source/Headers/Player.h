#pragma once

#include "Card.h"

class Player {
	bool isUser = { false };
	int cash{ 0 };
	deque<Card> playerCards;
	int cardsCenterX{ 0 }, 
		cardsCenterY{ 0 };

	void PlayerCardsCentered();
public:

	Player(const bool& isUser, const int& defaultCash = 500);
	~Player();

	void AddCash(const int& cash);
	void SubCash(const int& cash);
	void AddCard(const Card& card);

	deque<Card>& GetCards();
	void SetCardsCoord(const int& x, const int& y);
	int& GetCardCoordX();
	int& GetCardCoordY();

	void DrawCards(SDL_Renderer* render);
};