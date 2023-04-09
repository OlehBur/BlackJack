#include "Headers/Player.h"


void Player::AddCash(const int& cash) {
	this->cash += cash;
};

void Player::SubCash(const int& cash) {
	this->cash -= cash;
};

void Player::PlayerCardsCentered() {
	if (!playerCards.empty()) {
		int cardsX{ 0 },
			cardsY{ 0 };

		switch (playerPlacement) {
		case CARD_PLACE_RIGHT:
			cardsX = cardsCenter.x - playerCards.front().GetRectOnScreen().h / 3;
			cardsY = cardsCenter.y - (playerCards.size()-1) * (playerCards.front().GetRectOnScreen().w / 4);

			for (int i = 0; i < playerCards.size(); i++)
				playerCards.at(i).MoveToCoord(
					cardsX,
					cardsY + i * playerCards.front().GetRectOnScreen().w / 2);
			break;

		case CARD_PLACE_LEFT:
			cardsX = cardsCenter.x + playerCards.front().GetRectOnScreen().h / 3;
			cardsY = cardsCenter.y - (playerCards.size()-1) * (playerCards.front().GetRectOnScreen().w / 4);

			for (int i = 0; i < playerCards.size(); i++)
				playerCards.at(i).MoveToCoord(
					cardsX,
					cardsY + i * playerCards.front().GetRectOnScreen().w / 2);
			break;

		default:
			cardsX = cardsCenter.x - (playerCards.size() - 1) * (playerCards.front().GetRectOnScreen().w / 4);
			cardsY = cardsCenter.y - playerCards.front().GetRectOnScreen().h / 2;

			//change cards pos
			for (int i = 0; i < playerCards.size(); i++)
				playerCards.at(i).MoveToCoord(
					cardsX + i * playerCards.front().GetRectOnScreen().w / 2,
					cardsY);//AnimateMotion(to coord)
		}
		playerTittle.MoveToCoord(cardsX, cardsY);
	}
};

void Player::DiscardCards() {
	playerCards.clear();
};

void Player::CardsUpatePlacement(bool allCards) {
	if (allCards)
		for (int c = 0; c < playerCards.size(); c++)
			playerCards.at(c).SetPlacement(playerPlacement);

	else playerCards.back().SetPlacement(playerPlacement);
};

void Player::RemoveTittle() {
	cntAces = 0;
	playerTittle.MoveToCoord(card_value_def_coords.x, card_value_def_coords.y);
};

void Player::ChipsUpdatePlacement() {

	switch (playerPlacement) {
	case CARD_PLACE_RIGHT:
		chipCenter = { 
			int(cardsCenter.x - display.h/2.5),
			cardsCenter.y - display.h / 10 };
		break;

	case CARD_PLACE_LEFT:
		chipCenter = {
			int(cardsCenter.x + display.h / 3.5), 
			cardsCenter.y - display.h / 15 };
		break;

	default: 
		chipCenter = {
			cardsCenter.x, 
			int(cardsCenter.y - display.h / 2.4) };
	}

	chipPosInColumn = chipCenter.y; //default height for chip in column
}


void Player::ChipsUpdatePos() {
	if (betChips.size() > first_stack_chips
		&& betChips.size() < second_stack_chips + 1) {//second Column

		if (betChips.size() == first_stack_chips + 1)//perform operation only once
			chipPosInColumn += second_stack_chips * (betChips.size() - 1);//get value of the initial height

		switch (playerPlacement) {//different coordinate positions for different placements
		case CARD_PLACE_DEFAULT:
			betChips.back().MoveToCoord(
				chipCenter.x - betChips.back().GetWidth() / 1.2,
				chipPosInColumn - betChips.back().GetWidth() / 4);
			break;

		case CARD_PLACE_RIGHT:
			betChips.back().MoveToCoord(
				chipCenter.x + betChips.back().GetWidth() / 2, 
				chipPosInColumn + betChips.back().GetWidth() / 2);
			break;

		case CARD_PLACE_LEFT:
			betChips.back().MoveToCoord(
				chipCenter.x - betChips.back().GetWidth() / 2, 
				chipPosInColumn + betChips.back().GetWidth() / 2);
			break;
		}
	}

	else if (betChips.size() > second_stack_chips) {//third column
		if (betChips.size() == second_stack_chips+1)
			chipPosInColumn += second_stack_chips * (betChips.size() - 1);

		switch (playerPlacement) {
		case CARD_PLACE_DEFAULT:
			betChips.back().MoveToCoord(
				chipCenter.x - betChips.back().GetWidth() / 1.8,
				chipPosInColumn + betChips.back().GetWidth() / 10);
			break;

		case CARD_PLACE_RIGHT:
			betChips.back().MoveToCoord(
				chipCenter.x - betChips.back().GetWidth() / 3,
				chipPosInColumn + betChips.back().GetWidth() / 6);
			break;

		case CARD_PLACE_LEFT:
			betChips.back().MoveToCoord(
				chipCenter.x + betChips.back().GetWidth() / 4,
				chipPosInColumn + betChips.back().GetWidth() / 3);
			break;
		}

	}

	else//default first column
		betChips.back().MoveToCoord(
			chipCenter.x, 
			chipPosInColumn);

	chipPosInColumn -= chips_column_distance;//regardless of the column after each chip to raise the height
};

Player::Player(ScreenPlacement sp, 
	SDL_Renderer* render,
	const bool& isUser, 
	const int& defaultCash,
	const bool& isDealer) {

	this->isUser = isUser;
	this->isDealer = isDealer;
	this->cash = defaultCash;
	playerPlacement = sp;

	CardsUpatePlacement(true);
	this->playerTittle.InitFont(render, 
		card_value_def_coords.x,
		card_value_def_coords.y, 
		card_value_tittle_size);

	this->playerTittle.SetText("", render);
};

void Player::Destructor_Player/*~Player*/() {
	//for (int card = 0; card < playerCards.size(); card++)
	//	playerCards.at(card).Destructor_Card();
	playerCards.clear();

	//for (int chip = 0; chip < betChips.size(); chip++)
	//	betChips.at(chip).Destructor_Chip();
	betChips.clear();

	//playerTittle.~Tittle/*Destructor_Tittle*/();
};

bool Player::AddChip(const Chip& chip, const bool& isFree) {
	int buffCash = (isFree) ?
		chip_free_price : chip_default_price;//price per chip
	if (cash >= buffCash) {
		
		SubCash(buffCash);

		betChips.emplace_back(chip);

		ChipsUpdatePos();
		
		return true;
	}
	return false;
};

bool Player::SubChip(const bool& isCash) {
	if (betChips.size() > isCash) {//if you bust, chips reset to zero, if u click minus cash (isCash=true) - then the minimum number of chips is 1
		betChips.pop_back();
		if(isCash)
			AddCash(chip_default_price);

		chipPosInColumn = (betChips.empty()) ?
			chipCenter.y : 
			betChips.back().GetCoordY() - chips_column_distance;	//update position of the player's last chip 
		return true;
	}
	return false;
};

void Player::AddCard(const Card& card) {
	playerCards.emplace_back(card);
};

void Player::ConvertChipToCash() {
	while (!betChips.empty()) {
		betChips.pop_back();
		AddCash(200);

		chipPosInColumn = (betChips.empty()) ?
			chipCenter.y :
			betChips.back().GetCoordY() - chips_column_distance;	//update position of the player's last chip 
	}
};


Card& Player::GetLastCard() {
	if (!playerCards.empty())
		return playerCards.back();
};

bool& Player::IsUser() {
	return isUser;
};

bool& Player::IsDealer() {
	return isDealer;
};

bool& Player::IsMadeBet() {
	return isMadeBet;
};

bool& Player::IsStand() {
	return isStand;
};

bool& Player::IsBust() {
	return isBust;
};

void Player::SetBet(const bool& isMadeBet) {
	this->isMadeBet = isMadeBet;
};

void Player::SetStand(const bool& isStand) {
	this->isStand = isStand;
};

void Player::SetBust(const bool& isBust) {
	this->isBust = isBust;	
};

void Player::SetCardsCoord(const Point& coords) {
	cardsCenter.x = coords.x;
	cardsCenter.y = coords.y;

	ChipsUpdatePlacement();		
};

void Player::SetGameResult(const string& str, SDL_Renderer *render, SDL_Color color, SDL_Color colorOutline) {
	playerTittle.SetText(str, render, color, colorOutline);
};

Point& Player::GetCardCoord() {
	return cardsCenter;
};

ScreenPlacement Player::GetPlacement() {
	return playerPlacement;
};

int Player::GetCardsCount() {
	return playerCards.size();
};

int Player::GetCardsValue() {
	cntAces = 0;
	int cardsCnt = playerCards.size(), 
		cardsValue{ 0 };

	if (cardsCnt) {
		for (int c = 0; c < cardsCnt; c++)
			if (playerCards.at(c).GetValue())//if not ace
				cardsValue += playerCards.at(c).GetValue();

			else cntAces++;
		
		return cardsValue;
	}
	return LACK_OF_CARDS;
};

int& Player::GetAcesCount() {
	return cntAces;
};

int& Player::GetCash() {
	return cash;
};

int Player::GetChipsCount() {
	return betChips.size();
};

Chip Player::GetChip() {

	return betChips.back();
};

void Player::DrawCards(SDL_Renderer* render) {
	if (!playerCards.empty()) {
		for (int i = 0; i < playerCards.size(); i++)
			playerCards.at(i).Draw(render);

		DrawTittle(render);
	}
};

void Player::DrawChips(SDL_Renderer* render) {
	if (!betChips.empty())
		for (list <Chip>::iterator i = betChips.begin(); i != betChips.end(); advance(i,1))
				(*i).Draw(render);
};

void Player::DrawTittle(SDL_Renderer* render) {
	if (!isDealer && !isStand)//if not dealer and dont stand
		playerTittle.SetText(to_string(GetCardsValue() + GetAcesCount()), render, card_value_tittle_color);//draw card value
	
	else if(isDealer && isStand)//dealer shows his score only when he stops 
		playerTittle.SetText(to_string(GetCardsValue()), render, card_value_tittle_color);
	
	else if(isDealer && !isStand)
		playerTittle.SetText("", render, card_value_tittle_color);

	playerTittle.Draw(render);
}

