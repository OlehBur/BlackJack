#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include <deque>
#include <vector>

using namespace std;

typedef const int& Coordinate;

struct Point {
	int x, y;
};

class GameItems {
protected:
	//Point coord;
	float x, y;
	/*SDL_Rect positionR{ 0 };*/
public:
	virtual void Draw() {};//=0;

	void MoveToCoord(Coordinate x, Coordinate y);
	
	static SDL_Texture* LoadTexture(const std::string& filePath, SDL_Renderer* renderTarg);
	static void DrawText(SDL_Renderer* render, Coordinate x, Coordinate y, const string& str, const string fontPath = "Resource\\TTF\\text.ttf");


	float& GetCoordX();
	float& GetCoordY();
};