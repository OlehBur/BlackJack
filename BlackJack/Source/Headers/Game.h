#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <stack>
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
	Mix_Chunk* cardFlip_Song{ nullptr },
		*buttonClick_Song{ nullptr },
		*chip_Song{ nullptr };
	SDL_Event event{ 0 };

	SDL_DisplayMode display;

	stack<Card> cardPlayDeck/*[52]*/;
	vector <Button> buttons;
	vector <Player> players;
	//Player dealer= Player(false);

	int DECK_POS_X{ 0 }, DECK_POS_Y{ 0 };
	bool isRun{ false },
		isMusicOn{ true },
		distributionCards{ false },
		startBets{ false };
	bool topCardIsMove{ false };

	//timer
	float frameTime{ 0.0f },
		deltaTime{ 0.0f }, 
		playersTurnTime{ 0.0f },
		playersBetTime{ 0.0f };
	int prevTime{ 0 }, currentTime{ 0 };
	short int currentPlayer = { 0 }/*, pleyersCnt = 4*/;
	
	void DeckGeneration();

	//GamePlay
	void TakeCard(Player& player, bool isDealer = false);
	void SkipTake();
	void EndGame();
	void NPCGamePlay(Player& player);
	void NPCMadeBet(Player& player);

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