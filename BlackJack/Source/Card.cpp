#include "Headers/Card.h"


//void Card::SetCardSkin(const string& name = "Default") {
//	Card::skin = name;
//};

Card::Card(SuitType suit, CardType card, int x, int y) {
	this->suit = suit;
	this->type = card;
};

Card::~Card() {
	topTexture = nullptr;
	suitTexture = nullptr;

};

void Card::AnimateMotion() {

};

void Card::Draw(SDL_Renderer* render) {
	//SDL_RenderClear(render);
	SDL_RenderCopy(render, topTexture/*(isUpsideDown) ? topTexture : suitTexture*/, NULL, &positionR);

	//SDL_RenderPresent(render);
};

void Card::SetScaleTexture(const int& ScreenWidth) {
	int buffSize = positionR.w;
	if (positionR.w) {
		positionR.w = ScreenWidth/11;
		scale = (float)positionR.w / buffSize;
		positionR.h *= scale;
	}
	else {
		scale = .5;
		positionR.w *= scale;
		positionR.h *= scale;
	}
};

//void Card::DrawDeck(SDL_Renderer* render, int x, int y) {
//
//};

void Card::InitTexture(SDL_Renderer* render) {
	string buffPatch;
	string skin = "Default";

	buffPatch = "Resource\\Images\\Cards skins\\"+skin+"\\jack_of_clubs2.png";
	topTexture = GameItems::LoadTexture(buffPatch, render);
	//suitTexture = Game::LoadTexture("Resource\\Images\\background.png", render);
	SDL_QueryTexture(topTexture, NULL, NULL, &positionR.w, &positionR.h);
	positionR.w /= 2;
	positionR.h /= 2;
	positionR.x = GetCoordX() - positionR.w / 2;
	positionR.y = GetCoordY() - positionR.h / 2;
};

int& Card::GetSuit() {
	return suit;
};

int& Card::GetType() {
	return type;
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