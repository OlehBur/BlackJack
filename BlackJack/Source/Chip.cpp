#include "Headers/Chip.h"

bool Chip::InitTextures(SDL_Renderer* render) {
	//chipTextrue.reset(GameItems::LoadTexture("Resource\\Images\\Chips\\chips.png", render));
	chipTextrue = GameItems::LoadTexture("Resource\\Images\\Chips\\chips.png", render);
	SDL_QueryTexture(chipTextrue/*.get()*/, NULL, NULL, &textureR.w, &textureR.h);

	textureR.w *= chip_texture_scale;//centered texture

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


//Chip::Chip(const Chip& chip) {
//	isDealerChip = chip.isDealerChip;
//
//	/*chipTextrue.reset(chip.chipTextrue.get());*/
//	chipTextrue=chip.chipTextrue;
//};

Chip::Chip(const bool& isDealerChip, SDL_Renderer* render) {
	this->isDealerChip = isDealerChip;

	InitTextures(render);
};

//void Chip::Destructor_Chip() {
//	SDL_DestroyTexture(chipTextrue);
//	chipTextrue = nullptr;
//};

bool& Chip::IsDealerChip() {
	return isDealerChip;
};

void Chip::UpdateRect() {
	positionR.w = textureR.w * chip_texture_scale;
	positionR.h = textureR.h * chip_texture_scale;
	positionR.x = x - positionR.w * chip_texture_scale;
	positionR.y = y - positionR.h * chip_texture_scale;
};

void Chip::MoveToCoord(Coordinate x, Coordinate y) {
	positionR.x=this->x = x;
	positionR.y=this->y = y;
};

void Chip::Draw(SDL_Renderer* render) {
	SDL_RenderCopy(render,
		chipTextrue/*.get()*/,
		&textureR,
		&positionR);
}


int& Chip::GetWidth() {
	return positionR.w;
};
