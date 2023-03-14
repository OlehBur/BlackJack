#include "Headers/GameItems.h"

SDL_DisplayMode display;

void GameItems::DrawText(SDL_Renderer* render,
	Coordinate x, Coordinate y, const string& str, 
	const int& sizeText, const string fontPath){

	SDL_Surface* textS{ nullptr },
		* outlineS{ nullptr };
	SDL_Texture* textT{ nullptr };
	TTF_Font* appFont{ nullptr },
		* outlineFont{ nullptr };
	SDL_Rect textR{ NULL };
	
	int outlineSize = ((sizeText / 40)<1)?
		1 : sizeText / 40;
	
	//create fonts
	appFont = TTF_OpenFont(fontPath.c_str(), sizeText);
	outlineFont = TTF_OpenFont(fontPath.c_str(), sizeText);
	TTF_SetFontOutline(outlineFont, outlineSize);

	//create text render
	outlineS = TTF_RenderText_Blended(outlineFont, str.c_str(), { 102,0,0, 255 });
	textS = TTF_RenderText_Solid(appFont, str.c_str(), { 204,102,0,255 });
	//get new parametres for rect 
	textR = { 
		x- textS->w/2,
		y- textS->h/2, 
		textS->w, 
		textS->h 
	};
	
	//font draw
	textT = SDL_CreateTextureFromSurface(render, textS);
	SDL_FreeSurface(textS);
	SDL_RenderCopy(render, textT, NULL, &textR);

	//outline font draw
	textT = SDL_CreateTextureFromSurface(render, outlineS);
	SDL_FreeSurface(outlineS);
	textR.y-=3;//fix outline calculating error
	SDL_RenderCopy(render, textT, NULL, &textR);

	//SDL_SetSurfaceBlendMode(textS, SDL_BLENDMODE_BLEND);
	//SDL_BlitSurface(textS, NULL, outlineS, NULL/*&textR*/);

	textS = nullptr;
	outlineS = nullptr;
	
	SDL_DestroyTexture(textT);
	textT = nullptr;
	appFont = nullptr;
	outlineFont = nullptr;
}

SDL_Texture* GameItems::LoadTexture(const string& filePath, SDL_Renderer* renderTarg) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		SDL_Log(IMG_GetError());
	else {
		texture = SDL_CreateTextureFromSurface(renderTarg, surface);
		if (texture == NULL)
			SDL_Log(SDL_GetError());
	}

	SDL_FreeSurface(surface);
	return texture;
};


void GameItems::MoveToCoord(Coordinate x, Coordinate y) {
	this->x = x;
	this->y = y;

};


float& GameItems::GetCoordX() {
	return x;
};

float& GameItems::GetCoordY() {
	return y;
};