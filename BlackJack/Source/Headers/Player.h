#pragma once

#include "Card.h"
#include "Chip.h"

class Player {
	bool isUser{ false };

	deque<Card> playerCards;
	Point cardsCenter{ 0, 0 }, 
		chipCenter{ 0, 0 };
	int cntAces{ 0 };
	int playerPlacement{ 0 };

	int cash{ 0 };
	vector<Chip> betShips;

	void AddCash(const int& cash);
	void SubCash(const int& cash);

public:

	Player(ScreenPlacement sp, const bool& isUser = false, const int& defaultCash = 1000);
	~Player();


	void AddChip(const Chip& chip/*, const int& count=1*/);
	void SubChip();
	void AddCard(const Card& card);
	void PlayerCardsCentered();

	void CardsUpatePlacement(bool allCards = false);

	//bool& IsMadeBet()
	bool& IsUser();
	Card& GetLastCard();
	void SetCardsCoord(const Point& coords);
	Point& GetCardCoord();
	ScreenPlacement GetPlacement();
	/*
	return -1, when player doesnt have a cards
	*/
	int GetCardsCount();
	int GetCardsValue();
	int& GetAcesCount();
	int& GetCash();

	void DrawCards(SDL_Renderer* render);
	void DrawChips(SDL_Renderer* render);
};