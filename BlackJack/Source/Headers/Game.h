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


#define Waiting_Time(condition, time1, time2)	\
	((condition)?								\
		distributionCards_waiting_time :		\
		autoPlay_waiting_time)					\


constexpr float endRound_waiting_time = 60.0f;
constexpr float distributionCards_waiting_time = 5.0f;
constexpr float autoPlay_waiting_time = 40.0f;
constexpr float made_bet_waiting_time = 30.0f;
constexpr float deltaTime_koefficient = 100.0f;
constexpr float card_animation_update_time = 0.05f;

constexpr int dealer_default_cardsCnt = 2;
constexpr int min_allow_numb_cards = 8;
constexpr int winning_cards_value = 21;
constexpr int ace_max_value = 11;
constexpr int norm_dealer_card_values = 16;
constexpr int cardDeck_pos_deviation = 2;

constexpr float tittle_main_scale = 1 / 11.0f;
constexpr float header_first_scale = 0.8f;
constexpr float header_second_scale = 0.7f; 
constexpr float header_third_scale = 0.6f; 
constexpr float header_fourth_scale = 0.4f;
constexpr float header_fifth_scale = 0.3f;

constexpr float button_main_scale = 0.6f;
constexpr float button_addit_scale = 0.8f;
constexpr float button_small_scale = 0.3f;
constexpr float button_big_scale = 0.7f;

constexpr int double_prize_factor = 2;
constexpr float oneAndHalf_prize_factor = 1.5f;

constexpr SDL_Color player_stand_text_color = { 255, 255, 102, 255 };
constexpr SDL_Color player_stand_outline_color = { 102, 51, 0, 255 };
constexpr SDL_Color player_win_text_color = { 255, 128, 0, 255 };
constexpr SDL_Color player_win_outline_color = { 0,0,0,255 };
constexpr SDL_Color player_bust_text_color = { 128, 255, 0, 255 };
constexpr SDL_Color player_bust_outline_color = { 0,0,0,255 };
constexpr SDL_Color background_helpWnd_color = { 30, 10, 10, 100 };

constexpr int game_audio_frequency = 22050;
constexpr int game_audio_channels = 2;
constexpr int game_audio_chunk_size = 2048;


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