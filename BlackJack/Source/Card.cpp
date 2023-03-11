#include "Headers/Card.h"

string Card::skin;

void Card::SetCardSkin(const string& name = "Default") {
	skin = name;
};

void Card::UpdateRect() {
	positionR.x = x - positionR.w / 2;
	positionR.y = y - positionR.h / 2;
};

Card::Card(SuitType suit, CardType card, SDL_Renderer* render, ScreenPlacement sp, Coordinate x, Coordinate y) {
	this->suit = suit;
	this->type = card;

	this->x = x;
	this->y = y;

	SetPlacement(sp);
	InitTexture(render);
	UpdateRect();
};

void Card::Destructor_Card/*~Card*/() {
	SDL_DestroyTexture(topTexture);
	SDL_DestroyTexture(suitTexture);
	topTexture = nullptr;
	suitTexture = nullptr;
};

bool Card::AnimateMotion(const Point& p1, const float& deltatime, SDL_Renderer* render, bool isNormalPlacement) {
	Point buffP = p1;
	if(isNormalPlacement)
		buffP.y -= positionR.h / 2;
	//switch (sp) {
	//case CARD_PLACE_LEFT:
	//	buffP.x -= positionR.w / 2;
	//	break;
	//case CARD_PLACE_RIGHT:
	//	buffP.x += positionR.w / 2;
	//	break;
	//default:
	//	buffP.y -= positionR.h / 2;
	//}
		
		
	//buffP.x -= positionR.h / 2;
		//int buffCordX = this->x - x,
	//	buffCordY = this->y - y;
	//	float lenX = buffCordX / 100, 
	//	lenY = buffCordY / 100;

	//	Point beginP = p1;

	//while (beginP.x != this->x || beginP.y != this->y) {
	//	//this->x += x * deltatime/* * cardMovingSpeed*/ * lenX;
	//	//this->y += y * deltatime /** cardMovingSpeed */* lenY;
	//	beginP.x = round(beginP.x + lenX);
	//	beginP.y = round(beginP.y + lenY);
	//	Draw(render);
	//}

	Point difference = { buffP.x - this->x, buffP.y - this->y };

	if (difference.x == 0 || /*&& */difference.y == 0)
		return true;

	float distance = sqrt(difference.x * difference.x + difference.y * difference.y);
	if (distance < 10)
		return true;
	float coefficientX = difference.x / distance;
	float coefficientY = difference.y / distance;

	this->x += coefficientX * deltatime * cardMovingSpeed;
	this->y += coefficientY * deltatime * cardMovingSpeed;

	Draw(render);
	return false;
};

void Card::Draw(SDL_Renderer* render) {
	UpdateRect();
	SDL_RenderCopyEx(render,
		(isUpsideDown) ? suitTexture : topTexture,
		NULL,
		&positionR,
		angleRotaton,
		NULL,
		SDL_FLIP_NONE);
	//SDL_RenderCopy(render, (isUpsideDown) ? suitTexture : topTexture, NULL, &positionR);
	//SDL_RenderDrawRect(render, &positionR);
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

void Card::SetCardPos(Coordinate x, Coordinate y) {
	this->x = x;
	this->y = y;
	
	UpdateRect();
};

void Card::SetPlacement(ScreenPlacement sp) {
	switch (sp) {
	case CARD_PLACE_DEFAULT:
		angleRotaton = 0;
		break;

	case CARD_PLACE_RIGHT:
		angleRotaton = 270;
		break;

	case CARD_PLACE_LEFT:
		angleRotaton = 90;
		break;
	}
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

	//UpdateRect();
};

int& Card::GetSuit() {
	return suit;
};

int& Card::GetType() {
	return type;
};

int Card::GetValue() {
	if (type <= CARD_TYPE_TEN)
		return type-100;
	else if (type == CARD_TYPE_ACE)
		return 0/*-1*/;
	else
		return 10;
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