#pragma once

#include "Card.h"
#include "Chip.h"
#include "Tittles.h"

#define LACK_OF_CARDS -1


constexpr int first_stack_chips = 5;
constexpr int second_stack_chips = 10;

constexpr int chips_column_distance = 10;
constexpr int chip_default_price = 200;
constexpr int chip_free_price = 0;

constexpr int card_value_tittle_size = 40;
constexpr FPoint card_value_def_coords = { -100, -100 };

constexpr SDL_Color card_value_tittle_color = { 0,255,255,255 };


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
	int playerPlacement{ 0 },
		chipPosInColumn{ 0 };
	Tittle playerTittle;

	int cash{ 0 };
	list<Chip> betChips;

	void AddCash(const int& cash);
	void SubCash(const int& cash);

	void ChipsUpdatePlacement();

	/*updates position of the player's last chip when it is received*/
	void ChipsUpdatePos();

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
	void ConvertChipToCash();

	void PlayerCardsCentered();
	void DiscardCards();
	void CardsUpatePlacement(bool allCards = false);
	void RemoveTittle();

	void SetBet(const bool& isMadeBet);
	void SetStand(const bool& isStand);
	void SetBust(const bool& isBust);
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

	/* return -1, when player doesnt have a cards */
	int GetCardsCount();

	/* return a player cards value and calc aces cnt*/
	int GetCardsValue();
	int& GetAcesCount();
	int& GetCash();
	int GetChipsCount();
	Chip GetChip();

	void DrawCards(SDL_Renderer* render);
	void DrawChips(SDL_Renderer* render);

};