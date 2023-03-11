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
	cardFlip_Song = nullptr;
	buttonClick_Song = nullptr;
	chip_Song = nullptr;

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
	cardFlip_Song = Mix_LoadWAV("Resource\\Audio\\flipCard.mp3");
	buttonClick_Song = Mix_LoadWAV("Resource\\Audio\\buttonClick.wav");
	chip_Song = Mix_LoadWAV("Resource\\Audio\\chip.wav");
	
	//cardfs deck
	DeckGeneration();

	//buttons
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

	//players
	players.reserve(4);
	/*dealer = Player(false);*/
	//dealer.SetCardsCoord({ display.w / 2, display.h / 3 });
	
	players.push_back(Player(CARD_PLACE_DEFAULT));//dealer
	players.back().SetCardsCoord({ display.w / 2, display.h / 4 });

	players.push_back(Player(CARD_PLACE_RIGHT));
	players.back().SetCardsCoord({ display.w - display.w / 20, int(display.h / 1.7) });
	players.push_back(Player(CARD_PLACE_DEFAULT, true));
	players.back().SetCardsCoord({ display.w / 2, int(display.h / 1.01 )});
	players.push_back(Player(CARD_PLACE_LEFT));
	players.back().SetCardsCoord({ display.w / 20,  int(display.h / 1.7) });
	
	
	//currentPlayer = players.size()-1;
	//TakeCard(players.at(currentPlayer), true);//add card for dealer

	isRun = true;
	/*distributionCards*/startBets = true;
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
	/*if(distributionCards)
		TakeCard(dealer, true);*/
	if (players.at(0).GetCardsCount() == 2) //if dealer has two cards
		distributionCards = false;
};

void Game::MouseActivity(const SDL_Rect& mousePos, const bool& isClick) {

	for (int b=0; b<buttons.size(); b++/*auto button : buttons*/) 
		if (buttons.at(b).Interact(mousePos, isClick) 
			&& !topCardIsMove 
			&& players.at(currentPlayer).IsUser() 
			&& !distributionCards && !startBets) {
			Mix_PlayChannel(-1, buttonClick_Song, 0);

			switch (Button::currentButtonClicked/*GetClickedButton()*/) {
			case ClickedButton::HIT:
				TakeCard(players.at(currentPlayer));//always user iter
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

	//player chips
	//for(auto player : players)
	//	if(player.IsMadeBet())AddChip(const Chip & chip)

	if (startBets) {
		if(!players.at(currentPlayer).IsUser())
			playersBetTime += deltaTime;

		if (playersBetTime >= 35.0f) {
			playersBetTime = 0;
			NPCMadeBet(players.at(currentPlayer));
		}
	}
	else {
		//auto take cards
		//if (distributionCards)
		//	TakeCard(players.at(players.size()-1), true);
		if (!players.at(currentPlayer).IsUser() || distributionCards) {//waiting when turn not user
			playersTurnTime += deltaTime;
			if (playersTurnTime >= /*(distributionCards) ? 10.0f :*/ 40.0f) {
				playersTurnTime = 0;

				if (!distributionCards && !currentPlayer) //dealer turn during notStart game
					SkipTake();
				else if (!distributionCards)
					NPCGamePlay(players.at(currentPlayer));//Take or skip
				else if (distributionCards)
					TakeCard(players.at(currentPlayer), !currentPlayer);//if currentPlayer = 0 - is dealer
			}
		}

		//animation cards
		if (topCardIsMove) {//only when cards is moving
			frameTime += deltaTime;
			if (frameTime >= 0.05f) {
				frameTime = 0;
				if (players.at(currentPlayer).GetLastCard().AnimateMotion(players.at(currentPlayer).GetCardCoord(),
					deltaTime, render, !players.at(currentPlayer).GetPlacement())) {//placement return invert numb, if default(0) - 1(true)
					players.at(currentPlayer).PlayerCardsCentered();
					players.at(currentPlayer).CardsUpatePlacement();
					//players.at(currentPlayer).GetLastCard().SetPlacement(players.at(currentPlayer).GetPlacement());
					Mix_PlayChannel(-1, cardFlip_Song, 0);
					topCardIsMove = false;

					//if (!currentPlayer) {  //isDealer
					//	if (players.at(currentPlayer).GetCardsCount() >= 2) {
					//		distributionCards = false;
					//		SkipTake();
					//	}
					//}
					//else {//isn`t dealer
					SkipTake();
					//(currentPlayer < players.size() - 1) ?
					//	currentPlayer++ :
					//	currentPlayer = 0;

					/*EndGame();*///if last player - switch to first
				//}
				}
			}
		}

		//player cards
		for (int p = 0; p < players.size(); p++)
			players.at(p).DrawCards(render);
	}

	//gui elements
	for (int b = 0; b < buttons.size(); b++)
		buttons.at(b).Draw(render);


	SDL_RenderPresent(render);

}

void Game::TakeCard(Player& player, bool isDealer) {
	if (!cardPlayDeck.empty()
		/*&& !(isDealer *//*&& player.GetCardsCount()==2)*/) {//not dealer with 2 cards

		Mix_PlayChannel(-1, cardFlip_Song, 0);

		topCardIsMove = true;
		(isDealer && player.GetCardsCount() == 1) ?
			cardPlayDeck.top().SetUpsideDown(true) :
			cardPlayDeck.top().SetUpsideDown(false);

		player.AddCard(cardPlayDeck.top());

		cardPlayDeck.pop();

		//int val = player.GetCardsValue();

		/*if (player.GetCardsValue() >= 21)
			..*/
			//(currentPlayer == (pleyersCnt - 1)) ? currentPlayer = 0 : currentPlayer++;
	}
};

void Game::SkipTake() {
	(currentPlayer < players.size() - 1) ?
		currentPlayer++ :
		currentPlayer = 0;
};

void Game::NPCGamePlay(Player& player) {
	//if (rand() % 2 == 0) {
	//	if ((player.GetCardsValue() + player.GetAcesCount()) < 21) {
	//		TakeCard(player);
	//	}
	//	else {
	//		SkipTake();
	//	}
	//}
	//else {
	//	if ((player.GetCardsValue() + player.GetAcesCount() * 11) < 21) {
	//		TakeCard(player);
	//	}
	//	else {
	//		SkipTake();
	//	}
	//}
	if (rand() % 2 == 0)
		((player.GetCardsValue() + player.GetAcesCount()) < 21) ?
		TakeCard(player) :
		SkipTake();
	else ((player.GetCardsValue() + player.GetAcesCount() * 11) < 21) ?
		TakeCard(player) :
		SkipTake();
};

void Game::NPCMadeBet(Player& player) {
	for (int bets = 0; bets < rand() % (player.GetCash() / 200); bets++)
		player.AddChip(Chip(!currentPlayer, player.GetCardCoord().x, player.GetCardCoord().y, render));
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