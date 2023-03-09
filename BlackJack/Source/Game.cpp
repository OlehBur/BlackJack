#include "Headers/Game.h"
#include <Windows.h>


void Game::Error(const char str[]) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s\n", str);
};

Game::Game() {
	static bool isInit = false;

	if (!isInit) {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
		isInit = true;
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048/*~2kB*/) < 0)//44100
			Error("Mixer init error");//Mix_GetError()
	}
	else {
		Error("The Game class cannot have more than one object.");
		exit(0);
	}
};

Game::~Game() {

	for (auto button : buttons)
		button.Destructor_Button();
	buttons.clear();

	players.clear();

	while (!cardPlayDeck.empty()) {
		cardPlayDeck.top().Destructor_Card();
		cardPlayDeck.pop();
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	SDL_DestroyTexture(background);

	window = nullptr;
	render = nullptr;
	background = nullptr;
	musicBackground = nullptr;
	cardSong = nullptr;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
};

void Game::Init() {
	SDL_GetCurrentDisplayMode(0, &display);//get display params
	DECK_POS_X = display.w - display.w / 5;
	DECK_POS_Y = display.h / 5;

	window = SDL_CreateWindow("BlackJack",
		0, 0,
		display.w, display.h,
		SDL_WINDOW_OPENGL);
	render = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);

	background = GameItems::LoadTexture("Resource\\Images\\GUI\\background.png", render);

	//music
	musicBackground = Mix_LoadMUS("Resource\\Audio\\Stardust.mp3");
	//cardSong = Mix_LoadWAV("gin.wav");
	//buttonClick

	DeckGeneration();

	buttons.reserve(4);

	buttons.emplace_back(Button(render, BUTTON_TYPE_HIT, "HIT"));//Hit
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.2),	//x
		int(display.h - buttons.back().GetRect().h * 1.3));	//y 
		
	buttons.back().SetTittle("HIT");

	buttons.emplace_back(Button(render, BUTTON_TYPE_STAND, "STAND" )); //Stand
	buttons.back().SetCoord(
		int(buttons.back().GetRect().w * 1.2- buttons.back().GetRect().w),	//x
		int(display.h - buttons.back().GetRect().h * 1.3));	//y 

	buttons.back().SetTittle("STAND");

	players.push_back(Player(true));
	players.back().SetCardsCoord(display.w / 2, display.h - display.h / 11);
	players.push_back(Player(false));
	players.back().SetCardsCoord(display.w / 2, display.h / 11);

	isRun = true;
};

void Game::Stop() {
	isRun = false;
};

void Game::Update() {
	prevTime = currentTime;
	currentTime = SDL_GetTicks();
	deltaTime = (currentTime - prevTime) / 100.0f;

	if (!Mix_PlayingMusic() && isMusicOn)
		Mix_PlayMusic(musicBackground, -1);//2 param - cnt of rep, if -1 wil rep until don`t say stop
	/*else if (Mix_PausedMusic() && isMusicOn)
		Mix_ResumeMusic();
	else Mix_PauseMusic();*/
};

void Game::MouseActivity(const SDL_Rect& mousePos, const bool& isClick) {

	for (auto button : buttons) 
		if (button.Interact(mousePos, isClick)) {
			//playMusclickButt

			switch (Button::currentButtonClicked/*GetClickedButton()*/) {
			case ClickedButton::HIT:
				TakeCard(players.front());
				break;

			case ClickedButton::STAND:
				SkipTake();
				break;

			case ClickedButton::CASH_PLUS:
				break;

			case ClickedButton::CASH_MINUS:
				break;

			}
		}

};

void Game::DeckGeneration() {
	srand(time(0));
	int randIndex{ 0 };
	list<Card> DefaultDeck;
	list<Card>::iterator deckIter;

	for (int suit = CARD_SUIT_SPADE; suit <= CARD_SUIT_DIAMOND; suit++)
		for (int type = CARD_TYPE_TWO; type <= CARD_TYPE_ACE; type++) {
			DefaultDeck.emplace_back(Card(suit, type, render/*, DECK_POS_X, DECK_POS_Y*/));
			DefaultDeck.back().SetScaleTextureByScreen(display.w);
		}

	while (cardPlayDeck.size() != 52 && DefaultDeck.size() > 0) {//init card deck
		randIndex = (DefaultDeck.size() - 1) ?
			rand() % (DefaultDeck.size() - 1) : 0;
		deckIter = DefaultDeck.begin();
		advance(deckIter, randIndex);

		cardPlayDeck.push(*deckIter);

		DefaultDeck.erase(deckIter);//del its card from list
	}

	DefaultDeck.clear();
};


void Game::Draw() {
	/*frameTime += deltaTime;*/

	SDL_RenderClear(render);

	//draw Backgr
	SDL_RenderCopy(render, background, NULL, NULL);

	//draw deck cards
	if (cardPlayDeck.size() > 0) {
		cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);
			for (int card = 1; card < cardPlayDeck.size(); card++) {
				(card & 1) ?
					cardPlayDeck.top().MoveToCoord(cardPlayDeck.top().GetCoordX() + 2, cardPlayDeck.top().GetCoordY() - 2) :
					cardPlayDeck.top().MoveToCoord(cardPlayDeck.top().GetCoordX(), cardPlayDeck.top().GetCoordY());
					cardPlayDeck.top().Draw(render);
			}
	}

	//player cards
	for (int p = 0; p < players.size(); p++)
		players.at(p).DrawCards(render);

	//gui elements
	for (int b = 0; b < buttons.size(); b++)
		buttons.at(b).Draw(render);


	SDL_RenderPresent(render);

}

void Game::TakeCard(Player& player) {
	if (!cardPlayDeck.empty()) {
		//cardPlayDeck.top().AnimateMotion(player.GetCardCoordX(), player.GetCardCoordY(), deltaTime, render);
		cardPlayDeck.top().SetUpsideDown(false);
		player.AddCard(cardPlayDeck.top());
		cardPlayDeck.pop();
	}
};

void Game::SkipTake() {
	//index ++ on players vect
};

bool& Game::IsRun() {
	return isRun;
};

SDL_Event& Game::GetEvent() {
	return event;
};

SDL_Renderer* Game::GetRender() {
	return render;
};

float& Game::GetDeltaTime() {
	return deltaTime;
};