#include "Headers/Tittles.h"

Tittle::Tittle() {};

Tittle::Tittle(const string& str, SDL_Renderer* render,
	Coordinate x, Coordinate y,
	const int& sizeText,
	const string fontPath) {

	InitFont(render, x, y, sizeText);
	SetText(str, render);
};

//void Tittle::/*~Tittle*/Destructor_Tittle() {
//	/*TTF_CloseFont(appFont);
//	TTF_CloseFont(outlineFont);*/
//
//	/*textS = nullptr;
//	outlineS = nullptr;
//
//	SDL_DestroyTexture(textT);
//	SDL_DestroyTexture(outlineT);*/
//	/*textT = nullptr;*/
//};

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

	appFont.reset(TTF_OpenFont(fontPath.c_str(), sizeText));
	outlineFont.reset(TTF_OpenFont(fontPath.c_str(), sizeText));

	TTF_SetFontOutline(outlineFont.get(), outlineSize);
};

bool Tittle::SetText(const string& str, SDL_Renderer* render, const SDL_Color& fontColor, const SDL_Color& colorOutline) {
	if (appFont.get() == NULL)
		return false;
	
	else {
		outlineS.reset(TTF_RenderText_Blended(outlineFont.get(), str.c_str(), colorOutline));
		textS.reset(TTF_RenderText_Solid(appFont.get(), str.c_str(), fontColor));

		text = str;
		UpdateRect();

		//font
		textT.reset(SDL_CreateTextureFromSurface(render, textS.get()));
		
		//outline 
		outlineT.reset(SDL_CreateTextureFromSurface(render, outlineS.get()));

		return true;
	}
};

string& Tittle::GetText() {
	return text;
};

void Tittle::Draw(SDL_Renderer* render) {
	if (textT) {//is init texture
		SDL_RenderCopy(render, &(*textT), NULL, &textR);
		SDL_RenderCopy(render, &(*outlineT), NULL, &textR);
	}
};