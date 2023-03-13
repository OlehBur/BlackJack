#include "Headers/Card.h"

int Card::skin{ CARD_SKIN_DEFAULT };

void Card::ChangeCardSkin() {
	(skin < CARD_SKIN_CLASSIC) ?
		skin++ :
		skin = CARD_SKIN_DEFAULT;
};

void Card::UpdateRect() {
	positionR.x = x - positionR.w / 2;
	positionR.y = y - positionR.h / 2;
};

void Card::UpdateSkin() {
	switch (skin) {
	case CARD_SKIN_DEFAULT:

		break;
	case CARD_SKIN_MIDDLE_AGES:

		break;

	case CARD_SKIN_CLASSIC:

		break;
	}
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
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(topTexture[i]);
		SDL_DestroyTexture(suitTexture[i]);
		topTexture[i] = nullptr;
		suitTexture[i] = nullptr;
	}
};

bool Card::AnimateMotion(const Point& p1, const float& deltatime, SDL_Renderer* render, bool isNormalPlacement) {
	Point buffP = p1;
	if(isNormalPlacement)
		buffP.y -= positionR.h / 2;
	
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
		(isUpsideDown) ? suitTexture[skin - 80] : topTexture[skin - 80],
		NULL,
		&positionR,
		angleRotaton,
		NULL,
		SDL_FLIP_NONE);
};

void Card::SetScaleTexture(const float& scale) {
	this->scale = scale;

	SDL_QueryTexture(topTexture[skin-80], NULL, NULL, &positionR.w, &positionR.h);

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
	string buffPatchT[3],
		buffPatchS[3],
		skinFolder[3] = { "Default","MiddleAges","Classic" },
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

	for (int i = 0; i < 3; i++) {//ini all skins
		buffPatchT[i]= "Resource\\Images\\Cards skins\\" + skinFolder[i] + "\\" + name;
		topTexture[i] = GameItems::LoadTexture(buffPatchT[i], render);
		buffPatchS[i] = "Resource\\Images\\Cards skins\\" + skinFolder[i] + "\\" + "suit.png";
		suitTexture[i]= GameItems::LoadTexture(buffPatchS[i], render);
	}
	
	SDL_QueryTexture(topTexture[0], NULL, NULL, &positionR.w, &positionR.h);
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

bool& Card::IsUpsideDown() {
	return isUpsideDown;
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