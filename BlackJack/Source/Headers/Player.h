#pragma once

#include "Card.h"
#include "Chip.h"
#include "Tittles.h"


class Player {
	bool isUser{ false },
		isDealer{ false },
		isMadeBet{ false },
		isStand{ false }, 
		isBust{ false };;

	deque<Card> playerCards;
	Point cardsCenter{ 0, 0 }, 
		chipCenter{ 0, 0 };
	int cntAces{ 0 };
	int playerPlacement{ 0 };
	Tittle playerTittle;

	int cash{ 0 };
	vector<Chip> betShips;

	void AddCash(const int& cash);
	void SubCash(const int& cash);

	void ChipsUpdatePlacement();
	void DrawTittle(SDL_Renderer* render);

public:

	Player(ScreenPlacement sp,
		SDL_Renderer* render, 
		const bool& isUser = false,
		const int& defaultCash = 1000,
		const bool& isDealer = false);
	void Destructor_Player/*~Player*/();


	bool AddChip(const Chip& chip, const bool& isFree = false);
	bool SubChip(const bool& isCash=true);
	void AddCard(const Card& card);
	void PlayerCardsCentered();

	void CardsUpatePlacement(bool allCards = false);

	//bool& IsMadeBet()

	void SetBet();
	void SetStand();
	void SetBust();
	void SetCardsCoord(const Point& coords);
	void SetGameResult(const string& str, 
		SDL_Renderer* render, 
		SDL_Color color,
		SDL_Color colorOutline);

	bool& IsUser();
	bool& IsDealer();
	bool& IsMadeBet();
	bool& IsStand();
	bool& IsBust();
	Card& GetLastCard();
	Point& GetCardCoord();
	ScreenPlacement GetPlacement();
	/*
	return -1, when player doesnt have a cards
	*/
	int GetCardsCount();
	int GetCardsValue();
	int& GetAcesCount();
	int& GetCash();
	int GetChipsCount();
	Chip/*vector<Chip>/*::iterator*/ GetChip();

	void DrawCards(SDL_Renderer* render);
	void DrawChips(SDL_Renderer* render);
};