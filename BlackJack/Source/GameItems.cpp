#include "Headers/GameItems.h"

SDL_DisplayMode display;

void GameItems::DrawText(SDL_Renderer* render,
	Coordinate x, Coordinate y, const string& str, 
	const int& sizeText, const string fontPath){

	unique_ptr <SDL_Surface, SDL_SrfcDeleter> textS,
		outlineS;
	unique_ptr <SDL_Texture, SDL_TxtrDeleter> textT;
	unique_ptr <TTF_Font, SDL_FontDeleter> appFont,
		outlineFont;

	SDL_Rect textR{ NULL };
	
	int outlineSize = ((sizeText / 40)<1)?
		1 : sizeText / 40;
	
	//create fonts
	appFont.reset(TTF_OpenFont(fontPath.c_str(), sizeText));
	outlineFont.reset(TTF_OpenFont(fontPath.c_str(), sizeText));

	TTF_SetFontOutline(outlineFont.get(), outlineSize);

	//create text render
	outlineS.reset(TTF_RenderText_Blended(outlineFont.get(), str.c_str(), { 102,0,0, 255 }));
	textS.reset(TTF_RenderText_Solid(appFont.get(), str.c_str(), { 204,102,0,255 }));
	//get new parametres for rect 
	textR = { 
		x- textS->w/2,
		y- textS->h/2, 
		textS->w, 
		textS->h 
	};
	
	//font draw
	textT.reset(SDL_CreateTextureFromSurface(render, textS.get()));
	/*SDL_FreeSurface(textS);*/
	SDL_RenderCopy(render, textT.get(), NULL, &textR);

	//outline font draw
	textT.reset(SDL_CreateTextureFromSurface(render, outlineS.get()));
	//SDL_FreeSurface(outlineS);
	textR.y-=3;//fix outline calculating error
	SDL_RenderCopy(render, textT.get(), NULL, &textR);

	//SDL_SetSurfaceBlendMode(textS, SDL_BLENDMODE_BLEND);
	//SDL_BlitSurface(textS, NULL, outlineS, NULL/*&textR*/);

	//textS = nullptr;
	//outlineS = nullptr;
	//
	//SDL_DestroyTexture(textT);
	//textT = nullptr;
	//appFont = nullptr;
	//outlineFont = nullptr;
}

SDL_Texture* GameItems::LoadTexture(const string& filePath, SDL_Renderer* renderTarg) {
	static SDL_Texture *texture{ nullptr };
	unique_ptr <SDL_Surface, SDL_SrfcDeleter> surface;
	
	surface.reset(IMG_Load(filePath.c_str()));
		
	if (surface.get() == NULL)
		SDL_Log(IMG_GetError());

	else {
		texture = SDL_CreateTextureFromSurface(renderTarg, surface.get());

		if (texture == NULL)
			SDL_Log(SDL_GetError());
	}

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