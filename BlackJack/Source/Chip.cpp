#include "Headers/Chip.h"

bool Chip::InitTextures(SDL_Renderer* render) {
	chipTextrue = GameItems::LoadTexture("Resource\\Images\\Chips\\chips.png", render);
	SDL_QueryTexture(chipTextrue, NULL, NULL, &textureR.w, &textureR.h);

	textureR.w /= 2;

	if (isDealerChip)
		textureR.x = textureR.y = 0;
	else {
		textureR.x = textureR.w;
		textureR.y = 0;
	}

	UpdateRect();

	return (chipTextrue) ?
		true :
		false;
};

Chip::Chip(const bool& isDealerChip/*, Coordinate x, Coordinate y*/, SDL_Renderer* render) {
	this->isDealerChip = isDealerChip;
	//center.x = this->x = x;
	//center.y = this->y = y;

	InitTextures(render);
};

void Chip::Destructor_Chip() {
	SDL_DestroyTexture(chipTextrue);
	chipTextrue = nullptr;
};

bool& Chip::IsDealerChip() {
	return isDealerChip;
};

void Chip::UpdateRect() {
	positionR.w = textureR.w/2;//scale
	positionR.h = textureR.h/2;
	positionR.x = x - positionR.w / 2;
	positionR.y = y - positionR.h / 2;
};

void Chip::MoveToCoord(Coordinate x, Coordinate y) {
	positionR.x=this->x = x;
	positionR.y=this->y = y;
};

void Chip::Draw(SDL_Renderer* render) {
	SDL_RenderCopy(render,
		chipTextrue,
		&textureR,
		&positionR);
}


int& Chip::GetWidth() {
	return positionR.w;
};


//bool Chip::operator<(Chip& ñhip) {
//	return (isDealerChip < ñhip.isDealerChip) ?
//		true : false;
//};
//
//bool Chip::operator>(Chip& ñhip) {
//	return (isDealerChip > ñhip.isDealerChip) ?
//		true : false;
//};
//
//void Chip::operator=(Chip ñhip) {
//	ñhip.isDealerChip = isDealerChip;
//};