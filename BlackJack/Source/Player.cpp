#include "Headers/Player.h"


void Player::PlayerCardsCentered() {
	if (!playerCards.empty()) {
		int cardsX = cardsCenterX - (playerCards.size()-1) * (playerCards.front().GetRectOnScreen().w / 4),
			cardsY = cardsCenterY - playerCards.front().GetRectOnScreen().h / 2;

		//change cards pos
		for (int i = 0; i < playerCards.size(); i++)
			playerCards.at(i).SetCardPos(
				cardsX + i * playerCards.front().GetRectOnScreen().w / 2,
				cardsY);//AnimateMotion(to coord)
	}
};


Player::Player(const bool& isUser, const int& defaultCash) {
	this->isUser = isUser;
	this->cash = defaultCash;

};

Player::~Player() {
	playerCards.clear();
};


void Player::AddCash(const int& cash) {
	this->cash += cash;
};

void Player::SubCash(const int& cash) {
	this->cash -= cash;
};

void Player::AddCard(const Card& card) {
	playerCards.push_back(card);
	PlayerCardsCentered();
};


deque<Card>& Player::GetCards() {
	return playerCards;
};

void Player::SetCardsCoord(const int& x, const int& y) {
	cardsCenterX = x;
	cardsCenterY = y;
	/*PlayerCardsCentered();*/
};

int& Player::GetCardCoordX() {
	return cardsCenterX;
};

int& Player::GetCardCoordY() {
	return cardsCenterY;
};

void Player::DrawCards(SDL_Renderer* render) {
	if (playerCards.size())
		for (int i = 0; i < playerCards.size(); i++)
			playerCards.at(i).Draw(render);
};