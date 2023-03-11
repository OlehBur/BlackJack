#include "Headers/Chip.h"

bool Chip::InitTextures(SDL_Renderer* render) {
	chipTextrue = GameItems::LoadTexture("Resource\\Images\\GUI\\.png", render);
	SDL_QueryTexture(chipTextrue, NULL, NULL, &positionR.w, &positionR.h);

	UpdateRect();

	return (chipTextrue) ?
		true :
		false;
};

Chip::Chip(const bool& isDealerChip, Coordinate x, Coordinate Y, SDL_Renderer* render) {
	this->isDealerChip = isDealerChip;
	center.x = x;
	center.y = y;


	InitTextures(render);
};

Chip::~Chip() {
	SDL_DestroyTexture(chipTextrue);
	chipTextrue = nullptr;
};

bool& Chip::IsDealerChip() {
	return isDealerChip;
};

void Chip::UpdateRect() {
	positionR.x = x - positionR.w / 2;
	positionR.y = y - positionR.h / 2;
};

void Chip::Draw(SDL_Renderer* render) {
	SDL_RenderCopy(render,
		chipTextrue,
		NULL,
		&positionR);
}