#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <cstdlib>
#include <ctime>

#include <stack>
#include <map>
#include <algorithm>

#include "SmartPtr.h"
#include "Card.h"
#include "Chip.h"
#include "Button.h"
#include "Player.h"



class Game {
	//pointers
	unique_ptr <SDL_Window, SDL_WndDeleter> window;
	unique_ptr <SDL_Renderer, SDL_RndrDeleter> render;
	unique_ptr <SDL_Texture, SDL_TxtrDeleter> background;
	unique_ptr <Mix_Music, SDL_MusDeleter> musicBackground;
	unique_ptr <Mix_Chunk, SDL_ChunkDeleter> cardFlip_Song,
		buttonClick_Song,
		chip_Song;

	SDL_Event event{ 0 };

	stack<Card> cardPlayDeck;
	vector <Button> buttons;
	vector <Player> players;

	map <int, Tittle > tittles;

	int DECK_POS_X{ 0 }, DECK_POS_Y{ 0 };
	bool isRun{ false },
		isMusicOn{ true },
		distributionCards{ false },
		startBets{ false },
		isHelpOn{ false },
		isEndRound{ false }, 
		isEndGame{ false };
	bool topCardIsMove{ false };

	//timer
	float frameTime{ 0.0f },
		deltaTime{ 0.0f },
		playersTurnTime{ 0.0f },
		playersBetTime{ 0.0f },
		endRoundTime{ 0.0f };
	int prevTime{ 0 }, currentTime{ 0 };

	short int currentPlayerIndex = { 0 },
		cashButtonIndx{ 0 },
		textMaxSize{ 0 };
	
	void DeckGeneration();

	//GamePlay
	void AutoPlay();
	void TakeCard(Player& player, bool isDealer = false);
	void SkipTake();
	void SkipPlayer();
	void NextRound();
	void EndRound();
	void MadeBets();
	void NPCGamePlay(Player& player);
	void NPCMadeBet(Player& player);
	void WinChips(Player& player, const int& cntChips, const bool& isDoublePrice);
	void PlayerBust(Player& player);

	void DrawDeckCard();
	void DrawHelpTittles();
	void DrawUserBetTittles();
	void DrawUserTurnTittles();

	/*catches animation card requests and processes them*/
	void CardAnimIntercep();

	void MusicInit();
	void InitPlayers();
	void InitTittles();
	void InitButtons();

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