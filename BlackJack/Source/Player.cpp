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
				playerCards.at(i).SetCardPos(
					cardsX,
					cardsY + i * playerCards.front().GetRectOnScreen().w / 2);
			break;

		case CARD_PLACE_LEFT:
			cardsX = cardsCenter.x + playerCards.front().GetRectOnScreen().h / 3;
			cardsY = cardsCenter.y - (playerCards.size()-1) * (playerCards.front().GetRectOnScreen().w / 4);

			for (int i = 0; i < playerCards.size(); i++)
				playerCards.at(i).SetCardPos(
					cardsX,
					cardsY + i * playerCards.front().GetRectOnScreen().w / 2);
			break;

		default:
			cardsX = cardsCenter.x - (playerCards.size() - 1) * (playerCards.front().GetRectOnScreen().w / 4);
			cardsY = cardsCenter.y - playerCards.front().GetRectOnScreen().h / 2;

			//change cards pos
			for (int i = 0; i < playerCards.size(); i++)
				playerCards.at(i).SetCardPos(
					cardsX + i * playerCards.front().GetRectOnScreen().w / 2,
					cardsY);//AnimateMotion(to coord)
		}
	}
};

void Player::CardsUpatePlacement(bool allCards) {
	if (allCards)
		for (int c = 0; c < playerCards.size(); c++)
			playerCards.at(c).SetPlacement(playerPlacement);
	else playerCards.back().SetPlacement(playerPlacement);
};


Player::Player(ScreenPlacement sp, const bool& isUser, const int& defaultCash) {
	this->isUser = isUser;
	this->cash = defaultCash;
	playerPlacement = sp;

	betShips.reserve(2 * defaultCash / 200);

	CardsUpatePlacement(true);
};

Player::~Player() {
	playerCards.clear();
	betShips.clear();
};

void Player::AddChip(const Chip& chip) {
	betShips.emplace_back(chip);//betShips.push_back(chip);
};

void Player::SubChip() {
	if (betShips.size() > 1)
		betShips.pop_back();
};

void Player::AddCard(const Card& card) {
	playerCards.emplace_back(card);//push_back(card);
	//PlayerCardsCentered();
};


Card& Player::GetLastCard() {
	if (playerCards.size())
		return playerCards.back();
};

bool& Player::IsUser() {
	return isUser;
};

void Player::SetCardsCoord(const Point& coords) {
	cardsCenter.x = coords.x;
	cardsCenter.y = coords.y;
	/*PlayerCardsCentered();*/
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
	return -1;
};

int& Player::GetAcesCount() {
	return cntAces;
};

int& Player::GetCash() {
	return cash;
};

void Player::DrawCards(SDL_Renderer* render) {
	if (playerCards.size())
		for (int i = 0; i < playerCards.size(); i++)
			playerCards.at(i).Draw(render);
};

void Player::DrawChips(SDL_Renderer* render) {
	if (betShips.size())
		for (int i = 0; i < betShips.size(); i++)
			betShips.at(i).Draw(render);
};