#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include "Card.h"
#include "Chip.h"
#include "Button.h"
#include "Player.h"


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
	Mix_Music* musicBackground{ nullptr };
	Mix_Chunk* cardSong{ nullptr };
	SDL_Event event{ 0 };

	SDL_DisplayMode display;

	stack<Card> cardPlayDeck/*[52]*/;
	vector <Button> buttons;
	vector <Player> players;

	int DECK_POS_X{ 0 }, DECK_POS_Y{ 0 };
	bool isRun{ false }, isMusicOn{ true };

	//timer
	float frameTime{ 0 }, deltaTime{ 0 }, speed = 50.0f;
	int prevTime{ 0 }, currentTime{ 0 };
	
	void DeckGeneration();

	//GamePlay
	void TakeCard(Player& player);
	void SkipTake();

	//windows
	void InitWindows();
	void OpendWindow();

public:
	static void Error(const char str[]);

	Game();
	~Game();
		
	void Init();
	void Stop();
	void Update();
	void MouseActivity(const SDL_Rect& mousePos, const bool& isClick);
	void Draw();


	bool& IsRun();

	SDL_Event& GetEvent();
	SDL_Renderer* GetRender();
	float& GetDeltaTime();

};