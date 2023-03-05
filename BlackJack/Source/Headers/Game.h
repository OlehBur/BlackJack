#pragma once

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<cstdlib>
#include<ctime>
#include<string>
#include<stack>
#include<list>

#include"Card.h"

using namespace std;


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
	SDL_Event event;
	stack<Card> cardPlayDeck/*[52]*/; 
	bool isRun{ false };
	
	void DeckGeneration();

public:
	static void Error(const char str[]);
	static SDL_Texture* LoadTexture(const string& filePath, SDL_Renderer* renderTarg);

	Game();
	~Game();

	void Init();
	void Stop();
	void Draw();

	bool& IsRun();

	SDL_Event& GetEvent();
	SDL_Renderer* GetRender();

	
	
	
};