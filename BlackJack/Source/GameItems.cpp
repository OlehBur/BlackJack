#include "Headers/GameItems.h"


void GameItems::MoveToCoord(const int& x, const int& y) {
	this->x = x;
	this->y = y;
};

int& GameItems::GetCoordX() {
	return x;
};

int& GameItems::GetCoordY() {
	return y;
};