#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <list>
#include "Card.h"
#include "Chip.h"
#include "Button.h"


template <typename T> 
class Ptr {
	T* pointer;

public:
	Ptr(T* ptr) {pointer = ptr; }
	~Ptr() {delete pointer;}

	T& operator*() {return *pointer;}
};


class Game {
	SDL_Window* window{ nullptr };
	SDL_Renderer* render{ nullptr };
	SDL_Texture* background{ nullptr };
	SDL_Event event{ 0 };
	vector <Button> buttons;
	SDL_DisplayMode display;

	stack<Card> cardPlayDeck/*[52]*/;
	//vector<Card> cardPlayDeck;
	int DECK_POS_X{ 0 }, DECK_POS_Y{ 0 };
	bool isRun{ false };
	
	void DeckGeneration();

public:
	static void Error(const char str[]);

	Game();
	~Game();

	void Init();
	void Stop();
	void Update();
	void MouseActivity(const SDL_Rect& mousePos, const bool& isClick);
	void Draw();

	//windows
	void InitWindows();
	void OpendWindow();


	bool& IsRun();

	SDL_Event& GetEvent();
	SDL_Renderer* GetRender();

	
	
	
};