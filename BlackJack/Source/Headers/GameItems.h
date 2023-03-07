#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

using namespace std;

//struct Point {
//	int x, y;
//};

class GameItems {
protected:
	//Point coord;
	SDL_Rect positionR{ 0 };
public:
	virtual void Draw() {};//=0;

	void MoveToCoord(const int& x, const int& y);
	
	static SDL_Texture* LoadTexture(const std::string& filePath, SDL_Renderer* renderTarg);
	static void DrawText(SDL_Renderer* render, const int& x, const int& y, const string& str, const string fontPath = "Resource\\TTF\\text.ttf");


	int& GetCoordX();
	int& GetCoordY();
};