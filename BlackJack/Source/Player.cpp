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
		playerTittle.MoveToCoord(cardsX, cardsY);
		playerTittle.UpdateRect();
	}
};

void Player::CardsUpatePlacement(bool allCards) {
	if (allCards)
		for (int c = 0; c < playerCards.size(); c++)
			playerCards.at(c).SetPlacement(playerPlacement);
	else playerCards.back().SetPlacement(playerPlacement);
};

void Player::ChipsUpdatePlacement() {
	switch (playerPlacement) {
	case CARD_PLACE_RIGHT:
		chipCenter = { int(cardsCenter.x - display.h/2.5), cardsCenter.y - display.h / 10 };
		break;
	case CARD_PLACE_LEFT:
		chipCenter = { int(cardsCenter.x + display.h / 3.5), cardsCenter.y - display.h / 15 };
		break;
	default: 
		chipCenter = { cardsCenter.x, int(cardsCenter.y - display.h / 2.9) };
	}

}


Player::Player(ScreenPlacement sp, 
	SDL_Renderer* render,
	const bool& isUser, 
	const int& defaultCash,
	const bool& isDealer) {

	this->isUser = isUser;
	this->isDealer = isDealer;
	this->cash = defaultCash;
	playerPlacement = sp;

	betShips.reserve(2 * defaultCash / 200);

	CardsUpatePlacement(true);
	this->playerTittle.InitFont(render, -100, -100, 40);// = Tittle(to_string(playerCards.size()), render, NULL, NULL, 20);
	this->playerTittle.SetText(to_string(playerCards.size()), render);
};

void Player::Destructor_Player/*~Player*/() {
	for (int card = 0; card < playerCards.size(); card++)
		playerCards.at(card).Destructor_Card();
	playerCards.clear();

	for (int chip = 0; chip < betShips.size(); chip++)
		betShips.at(chip).Destructor_Chip();
	betShips.clear();

	playerTittle.Destructor_Tittle();
};

bool Player::AddChip(const Chip& chip, const bool& isFree) {
	int buffCash = (isFree) ? 0 : 200;//price per chip
	if (cash >= buffCash) {
		cash -= buffCash;

		betShips.emplace_back(chip);

		if (betShips.size() > 5) {//for more columns
			if(betShips.size() == 6)//perform operation only once
				chipCenter.y += 10 * (betShips.size() - 1);//get value of the initial height
			
			switch (playerPlacement) {//different coordinate positions for different placements
			case CARD_PLACE_DEFAULT:
				betShips.back().MoveToCoord(chipCenter.x - betShips.back().GetWidth()/1.3, chipCenter.y - betShips.back().GetWidth()/1.5);
				break;

			case CARD_PLACE_RIGHT:
				betShips.back().MoveToCoord(chipCenter.x + betShips.back().GetWidth()/2, chipCenter.y + betShips.back().GetWidth() / 2);
				break;

			case CARD_PLACE_LEFT:
				betShips.back().MoveToCoord(chipCenter.x - betShips.back().GetWidth() / 2, chipCenter.y + betShips.back().GetWidth()/2);
				break;
			}

			}
		else 
			betShips.back().MoveToCoord(chipCenter.x, chipCenter.y);
		chipCenter.y -= 10;//regardless of the column after each chip to raise the height
		return true;
	}
	return false;
};

bool Player::SubChip() {
	if (betShips.size() > 1) {
		betShips.pop_back();
		cash += 200;
		chipCenter.y += 10;
		return true;
	}
	return false;
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

bool& Player::IsDealer() {
	return isDealer;
};

bool& Player::IsMadeBet() {
	return isMadeBet;
};

bool& Player::IsStand() {
	return isStand;
};

void Player::SetBet() {
	isMadeBet = true;
};

void Player::SetStand() {
	isStand = true;
};

void Player::SetCardsCoord(const Point& coords) {
	cardsCenter.x = coords.x;
	cardsCenter.y = coords.y;
	/*PlayerCardsCentered();*/
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
	return -1;
};

int& Player::GetAcesCount() {
	return cntAces;
};

int& Player::GetCash() {
	return cash;
};

vector<Chip>/*::iterator*/ Player::GetChips() {
	return betShips/*.begin()*/;
};

void Player::DrawCards(SDL_Renderer* render) {
	if (playerCards.size()) {
		for (int i = 0; i < playerCards.size(); i++)
			playerCards.at(i).Draw(render);

		DrawTittle(render);
	}
};

void Player::DrawChips(SDL_Renderer* render) {
	if (betShips.size())
		for (int i = 0; i < betShips.size(); i++)
				betShips.at(i).Draw(render);
};

void Player::DrawTittle(SDL_Renderer* render) {
	if (!isDealer && !isStand)//if not dealer and dont stand
		playerTittle.SetText(to_string(GetCardsValue() + GetAcesCount()), render, { 0,255,255,255 });//draw card value
	playerTittle.Draw(render);
}