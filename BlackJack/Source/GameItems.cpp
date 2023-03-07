#include "Headers/GameItems.h"

void GameItems::DrawText(SDL_Renderer* render, const int& x, const int& y, const string& str, const string fontPath) {
	TTF_Font* appFont = TTF_OpenFont(fontPath.c_str(), 20);
	if (appFont == NULL)
		TTF_GetError();
	SDL_Color textColor = { 121,55,0,255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(appFont, str.c_str(), textColor);
	SDL_Texture* textT = SDL_CreateTextureFromSurface(render, textSurface);
	SDL_Rect textRect = { x,y,0,0 };

	
	SDL_QueryTexture(textT, NULL, NULL, &textRect.w, &textRect.h);//get w h text

//TTF_RenderText_Shaded(appFont, str.c_str(), textColor, {0,0,0,255});
	SDL_RenderCopy(render, textT, NULL, &textRect);//draw text

	SDL_FreeSurface(textSurface);//del data
	textSurface = nullptr;
	SDL_DestroyTexture(textT);
	textT = nullptr;
	appFont = nullptr;
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


void GameItems::MoveToCoord(const int& x, const int& y) {
	positionR.x = x;
	positionR.y = y;

};


int& GameItems::GetCoordX() {
	return positionR.x;
};

int& GameItems::GetCoordY() {
	return positionR.y;
};