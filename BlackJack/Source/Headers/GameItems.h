#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include <deque>
#include <vector>

#include "SmartPtr.h"


using namespace std;

typedef const int& Coordinate;

struct Point {
	int x{ 0 }, y{ 0 };
};


extern SDL_DisplayMode display;


class GameItems {

protected:
	float x{ 0.0f }, y{ 0.0f };

public:
	virtual void Draw() {};
	virtual void MoveToCoord(Coordinate x, Coordinate y);
	
	static SDL_Texture* LoadTexture(const std::string& filePath, SDL_Renderer* renderTarg);
	static void DrawText(SDL_Renderer* render,
		Coordinate x, Coordinate y, const string& str,
		const int& sizeText = 40,
		const string fontPath = "Resource\\TTF\\text.ttf");

	float& GetCoordX();
	float& GetCoordY();
};