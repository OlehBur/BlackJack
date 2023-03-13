#include "Headers/Tittles.h"

Tittle::Tittle() {};

Tittle::Tittle(const string& str, SDL_Renderer* render,
	Coordinate x, Coordinate y,
	const int& sizeText,
	const string fontPath) {

	InitFont(render, x, y, sizeText);
	SetText(str, render);
};

void Tittle::/*~Tittle*/Destructor_Tittle() {
	textS = nullptr;
	outlineS = nullptr;

	SDL_DestroyTexture(textT);
	SDL_DestroyTexture(outlineT);
	textT = nullptr;
	//appFont = nullptr;
	//outlineFont = nullptr;
};

void Tittle::UpdateRect() {
	if (textS)
		textR = {
		(int)x - textS->w / 2,
		(int)y - textS->h / 2,
		textS->w,
		textS->h
	};
}

void Tittle::InitFont(SDL_Renderer* render,
	Coordinate x, Coordinate y,
	const int& sizeText,
	const string fontPath) {

	MoveToCoord(x, y);

	int outlineSize = ((sizeText / 30) < 1) ?
		1 : sizeText / 30;

	appFont = TTF_OpenFont(fontPath.c_str(), sizeText);
	outlineFont = TTF_OpenFont(fontPath.c_str(), sizeText);

	TTF_SetFontOutline(outlineFont, outlineSize);
};

bool Tittle::SetText(const string& str, SDL_Renderer* render, const SDL_Color& fontColor, const SDL_Color& colorOutline) {
	if (appFont == NULL)
		return false;
	else {
		if (outlineS) 
			Destructor_Tittle();//preventing a memory leak when called again

		outlineS = TTF_RenderText_Blended(outlineFont, str.c_str(), colorOutline);
		textS = TTF_RenderText_Solid(appFont, str.c_str(), fontColor);

		text = str;
		UpdateRect();

		//font
		textT = SDL_CreateTextureFromSurface(render, textS);
		SDL_FreeSurface(textS);
		//outline 
		outlineT = SDL_CreateTextureFromSurface(render, outlineS);
		SDL_FreeSurface(outlineS);

		return true;
	}
};

string& Tittle::GetText() {
	return text;
};

void Tittle::Draw(SDL_Renderer* render) {
	if (textT) {//is init texture
		SDL_RenderCopy(render, textT, NULL, &textR);
		SDL_RenderCopy(render, outlineT, NULL, &textR);
	}
};