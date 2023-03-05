#include"Headers/Card.h"


Card::Card(SuitType suit, CardType card) {
	this->suit = suit;
	this->type = card;
};


void Card::AnimateMotion() {

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

Card Card::operator=(Card card) {
	return card;
	/*suit = card.GetSuit();
	type = card.GetType();*/
};