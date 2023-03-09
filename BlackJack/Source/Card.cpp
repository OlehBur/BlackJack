#include "Headers/Card.h"

string Card::skin;

void Card::SetCardSkin(const string& name = "Default") {
	skin = name;
};

void Card::UpdateRect() {
	positionR.x = x - positionR.w / 2;
	positionR.y = y - positionR.h / 2;
};

Card::Card(SuitType suit, CardType card, SDL_Renderer* render, const int& x, const int& y) {
	this->suit = suit;
	this->type = card;

	this->x = x;
	this->y = y;

	InitTexture(render);
};

void Card::Destructor_Card/*~Card*/() {
	SDL_DestroyTexture(topTexture);
	SDL_DestroyTexture(suitTexture);
	topTexture = nullptr;
	suitTexture = nullptr;
};

void Card::AnimateMotion(const int& x, const int& y, const float& deltatime, SDL_Renderer* render) {
	while (/*positionR.x - positionR.w / 2 != x
		&& positionR.y - positionR.h / 2 != y*/positionR.x>20) {
		this->x -= 1 * deltatime;
		this->y += 1 * deltatime;
		Draw(render);
	}
};

void Card::Draw(SDL_Renderer* render) {
	UpdateRect();
	SDL_RenderCopy(render, (isUpsideDown) ? suitTexture : topTexture, NULL, &positionR);
	SDL_RenderDrawRect(render, &positionR);
};

void Card::SetScaleTexture(const float& scale) {
	this->scale = scale;

	SDL_QueryTexture(topTexture, NULL, NULL, &positionR.w, &positionR.h);

	positionR.w *= scale;
	positionR.h *= scale;
};

void Card::SetScaleTextureByScreen(const int& screenWidth) {
	int buffSize = positionR.w;
		positionR.w = screenWidth / 11;
		scale = (float)positionR.w / buffSize;
		positionR.h *= scale;
};

void Card::SetCardPos(const int& x, const int& y) {
	this->x = x;
	this->y = y;
	
	UpdateRect();
};

void Card::SetUpsideDown(const bool& isUpsideDown) {
	this->isUpsideDown = isUpsideDown;
};

void Card::InitTexture(SDL_Renderer* render) {
	string buffPatchT="",
		buffPatchS="",
		skin = "Default",
		name = "";

	switch (suit) {
	case CARD_SUIT_SPADE:
		name = to_string(type - 100) + "_of_spades.png";
		break;
	case CARD_SUIT_CLUB:
		name = to_string(type - 100) + "_of_clubs.png";
		break;
	case CARD_SUIT_HEART:
		name = to_string(type - 100) + "_of_hearts.png";
		break;
	case CARD_SUIT_DIAMOND:
		name = to_string(type - 100) + "_of_diamonds.png";
		break;
	}

	buffPatchT = "Resource\\Images\\Cards skins\\" + skin + "\\" + name;
	topTexture = GameItems::LoadTexture(buffPatchT, render);
	buffPatchS = "Resource\\Images\\Cards skins\\" + skin + "\\"+"suit.png";
	suitTexture = GameItems::LoadTexture(buffPatchS, render);
	SDL_QueryTexture(topTexture, NULL, NULL, &positionR.w, &positionR.h);
	positionR.w *= scale;
	positionR.h *= scale;

	UpdateRect();
};

int& Card::GetSuit() {
	return suit;
};

int& Card::GetType() {
	return type;
};

SDL_Rect& Card::GetRectOnScreen() {
	return positionR;
};

bool Card::operator<(Card& card) {
	return 	(type < card.GetType()) ?
		true :
		false;
};

bool Card::operator>(Card& card) {
	return (type > card.GetType()) ?
		true :
		false;
};

void /*Card&*/ Card::operator=(Card card) {
	suit = card.GetSuit();
	type = card.GetType();
	//return card;
};