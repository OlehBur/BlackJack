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
		TTF_Init();
	}
	else {
		Error("The Game class cannot have more than one object.");
		exit(0);
	}
};

Game::~Game() {
	for (int b = 0; b < buttons.size(); b++)
		buttons.at(b).Destructor_Button();
	buttons.clear();

	for (map <int, Tittle >::iterator t = tittles.begin(); t!=tittles.end(); advance(t, 1))
		t->second.Destructor_Tittle();
	tittles.clear();

	for (int p = 0; p < players.size(); p++)
		players.at(p).Destructor_Player();
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


void Game::CardAnimIntercep() {
	if (topCardIsMove) {//only when cards is moving
		frameTime += deltaTime;
		if (frameTime >= 0.05f) {
			frameTime = 0;
			if (players.at(currentPlayer).GetLastCard().AnimateMotion(players.at(currentPlayer).GetCardCoord(),
				deltaTime, render, !players.at(currentPlayer).GetPlacement())) {//placement return invert numb, if default(0) - 1(true)
				players.at(currentPlayer).PlayerCardsCentered();
				players.at(currentPlayer).CardsUpatePlacement();

				topCardIsMove = false;

				SkipPlayer();
			}
		}
	}
};


void Game::MusicInit() {
	musicBackground = Mix_LoadMUS("Resource\\Audio\\Stardust.mp3");
	cardFlip_Song = Mix_LoadWAV("Resource\\Audio\\flipCard.mp3");
	buttonClick_Song = Mix_LoadWAV("Resource\\Audio\\buttonClick.wav");
	chip_Song = Mix_LoadWAV("Resource\\Audio\\chip.wav");
};

void Game::InitPlayers() {
	players.reserve(4);

	players.push_back(Player(CARD_PLACE_DEFAULT, render, false, 200, true));//dealer
	players.back().SetCardsCoord({
		display.w / 2,
		display.h / 4 });

	players.push_back(Player(CARD_PLACE_RIGHT, render));
	players.back().SetCardsCoord({ 
		display.w - display.w / 20,
		int(display.h / 1.9) });

	players.push_back(Player(CARD_PLACE_DEFAULT, render, true));
	players.back().SetCardsCoord({
		display.w / 2, 
		int(display.h / 1.01) });

	players.push_back(Player(CARD_PLACE_LEFT, render));
	players.back().SetCardsCoord({
		display.w / 20, 
		int(display.h / 1.9) });
};

void Game::InitTittles() {
		//turns help
	tittles.emplace(TITTLE_YOUR_TURN, Tittle());
	tittles[TITTLE_YOUR_TURN].InitFont(render,
		display.w / 2,
		display.h / 2.2, textMaxSize/*100*/);
	tittles[TITTLE_YOUR_TURN].SetText("Your turn, player!", render);

	tittles.emplace(TITTLE_MADE_BET, Tittle());
	tittles[TITTLE_MADE_BET].InitFont(render,
		display.w / 2,
		display.h / 1.8, textMaxSize * .6/*60*/);
	tittles[TITTLE_MADE_BET].SetText("Place your bet by clicking on \"+\" or \"-\".", render);

	tittles.emplace(TITTLE_MADE_BET1, Tittle());
	tittles[TITTLE_MADE_BET1].InitFont(render, 
		display.w / 2,
		display.h / 1.6, textMaxSize * .6/*60*/);
	tittles[TITTLE_MADE_BET1].SetText("Then click on your cashe to confirm the bet.", render);

	tittles.emplace(TITTLE_TAKE_OR_SKIP, Tittle());
	tittles[TITTLE_TAKE_OR_SKIP].InitFont(render,
		display.w / 2,
		display.h / 1.8, textMaxSize * .6/*60*/);
	tittles[TITTLE_TAKE_OR_SKIP].SetText("You can take a card by clicking on the \"HIT\"", render);

	tittles.emplace(TITTLE_TAKE_OR_SKIP1, Tittle());
	tittles[TITTLE_TAKE_OR_SKIP1].InitFont(render,
		display.w / 2,
		display.h / 1.6, textMaxSize * .6/*60*/);
	tittles[TITTLE_TAKE_OR_SKIP1].SetText(" or miss a turn by clicking on the \"STAND\"", render);

		//help wnd
	const string helpText[] = {
		"The essence of blackjack is to get a set of cards greater than the dealer's",
		"set of cards but less than 21. if you get 21 points, your bet is doubled. If",
		"the dealer gets less than 17 points, he takes an additional card, and if he",
		"gets more than 21, then all players get X2 their bets if during the game you",
		"got exactly 21 points, then you automatically get X1.5 prize and are eliminated",
		"from the round Number cards have corresponding points, face cards - 10, and ",
		"aces 1 or 11 at the player's choice (in the game - automatically in your favor)."
	};

	tittles.emplace(TITTLE_HELP, Tittle());
	tittles[TITTLE_HELP].InitFont(render, 
		display.w / 2,
		display.h / 3.85, textMaxSize/*100*/);
	tittles[TITTLE_HELP].SetText("BLACKJACK HELPER", render);

	for (int helpTittle = TITTLE_HELP1, cntIndex{ 0 }; helpTittle <= TITTLE_HELP7; helpTittle++, cntIndex++) {
		tittles.emplace(helpTittle, Tittle());
		tittles[helpTittle].InitFont(render,
			display.w / 2,
			display.h / 3 + (display.h / 15) * cntIndex, textMaxSize * .4/*40*/);
		tittles[helpTittle].SetText(helpText[cntIndex], render);
	}
	
		//end round message
	tittles.emplace(TITTLE_GAME_END_ROUND, Tittle());
	tittles[TITTLE_GAME_END_ROUND].InitFont(render,
		display.w / 2,
		display.h / 2.1, textMaxSize/*100*/);
	tittles[TITTLE_GAME_END_ROUND].SetText("- Round is complete -", render);
};

void Game::InitButtons() {
	buttons.reserve(4);

	buttons.emplace_back(Button(render, BUTTON_TYPE_HIT, textMaxSize * .6/*60*/));//Hit
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.2),	//x
		int(display.h - buttons.back().GetRect().h * 2.1));	//y 

	buttons.back().SetTittle("HIT", render);

	buttons.emplace_back(Button(render, BUTTON_TYPE_STAND, textMaxSize * .6/*60*/)); //Stand
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.2/*buttons.back().GetRect().w * 1.2 - buttons.back().GetRect().w*/),	//x
		int(display.h - buttons.back().GetRect().h * 1.1));	//y 

	buttons.back().SetTittle("STAND", render);

	buttons.emplace_back(Button(render, BUTTON_TYPE_CASH, textMaxSize * .6/*60*/)); //Cash
	buttons.back().SetCoord(
		display.w/20,
		int(display.h-buttons.back().GetRect().h * 2.1));
	buttons.back().SetTittle("0 $", render); 
	cashButtonIndx = buttons.size() - 1;

	buttons.emplace_back(Button(render, BUTTON_TYPE_CASH_MINUS, textMaxSize * .8/*80*/)); //minus
	buttons.back().SetCoord(
		display.w / 20,
		int(display.h-buttons.back().GetRect().h * 1.1));
	buttons.back().SetTittle("-", render);


	buttons.emplace_back(Button(render, BUTTON_TYPE_CASH_PLUS, textMaxSize * .7/*70*/)); //plus
	buttons.back().SetCoord(
		display.w / 20 + buttons.at(0).GetRect().w- buttons.back().GetRect().w,//0 - index of default button such as cash. align along the edge of this button
		int(display.h-buttons.back().GetRect().h * 1.1));
	buttons.back().SetTittle("+", render);


	buttons.emplace_back(Button(render, BUTTON_TYPE_CHANGE_SKIN, textMaxSize * .3/*30*/)); //change skin
	buttons.back().SetCoord(
		int(buttons.back().GetRect().w / 2 * .3),
		display.h / 15);
	buttons.back().SetTittle("Change Card Skin", render);


	buttons.emplace_back(Button(render, BUTTON_TYPE_HELP, textMaxSize * .7/*70*/)); //help
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.1),
		display.h / 15);
	buttons.back().SetTittle("?", render);
};


void Game::Init() {
	SDL_GetCurrentDisplayMode(0, &display);//get display params
	DECK_POS_X = display.w - display.w / 5;
	DECK_POS_Y = display.h / 5;

	textMaxSize = display.h / 11;

	window = SDL_CreateWindow("BlackJack",
		0, 0,
		display.w, display.h,
		SDL_WINDOW_OPENGL);
	render = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);

	background = GameItems::LoadTexture("Resource\\Images\\GUI\\background.png", render);

	//music
	MusicInit();
	
	//cardfs deck
	DeckGeneration();

		//gui
	//buttons
	InitButtons();
	
	//titles
	InitTittles();

	//players
	InitPlayers();

	isRun = true;
	distributionCards = false;
	startBets = true;
};

void Game::Stop() {
	isRun = false;
};

void Game::Update() {
	int playerCardsValue{ 0 },
		playerAcesCnt{ 0 };

	prevTime = currentTime;
	currentTime = SDL_GetTicks();
	deltaTime = (currentTime - prevTime) / 100.0f;

	if (!Mix_PlayingMusic() && isMusicOn)
		Mix_PlayMusic(musicBackground, -1);//2 param - cnt of rep, if -1 wil rep until don`t say stop

	MadeBets();

	if (players.at(0).GetCardsCount() >= 2) //if dealer has two cards
		distributionCards = false;

	for (int p = 1; p < players.size(); p++) {
		playerCardsValue = players.at(p).GetCardsValue();
		playerAcesCnt = players.at(p).GetAcesCount();

		if ((playerCardsValue + playerAcesCnt == 21 //if player has a 21 points
			|| playerCardsValue + playerAcesCnt*11 == 21)
			&& !players.at(p).IsBust())//if player not eliminated

			WinChips(players.at(p), players.at(p).GetChipsCount()*1.5, false);// add win chips x1.5 to player if he has a 21 points
		
		else if (players.at(p).GetCardsValue() > 21 && !players.at(p).IsBust())
			PlayerBust(players.at(p));
	}

	int playersBetCnt = 0;
	int playersStandCnt = 0;

	for (auto player : players) {
		(player.IsMadeBet()) ?
			playersBetCnt++ : playersBetCnt = 0;
		(player.IsStand()) ?
			playersStandCnt++ : playersStandCnt = 0;
	}

	if (playersBetCnt == players.size() && startBets) {//checking if everyone has bid
		startBets = false;
		distributionCards = true;
	}

	if (playersStandCnt >= players.size() - 1) {//checking if everyone clicked the stand except the dealer
		EndGame();
		isEndGame = true;
	}
	
};

void Game::MouseActivity(const SDL_Rect& mousePos, const bool& isClick) {

	for (int b=0; b<buttons.size(); b++/*auto button : buttons*/) 
		if (buttons.at(b).Interact(mousePos, isClick)) {
			
			if(!isHelpOn)
				Mix_PlayChannel(-1, buttonClick_Song, 0);

			switch (Button::currentButtonClicked) {
			case ClickedButton::HIT:
				if(!topCardIsMove 
					&& players.at(currentPlayer).IsUser() 
					&& !players.at(currentPlayer).IsStand()
					&& !distributionCards 
					&& !startBets && !isHelpOn)
				TakeCard(players.at(currentPlayer));//always user iter
				break;

			case ClickedButton::STAND:
				if (!topCardIsMove
					&& players.at(currentPlayer).IsUser()
					&& !players.at(currentPlayer).IsStand()
					&& !distributionCards
					&& !startBets && !isHelpOn)
					
					SkipTake();
				break;

			case ClickedButton::CASH_PLUS:
				if(startBets && players.at(currentPlayer).IsUser() && !isHelpOn)
					players.at(currentPlayer).AddChip(Chip(false, render));
				break;

			case ClickedButton::CASH_MINUS:
				if (startBets && players.at(currentPlayer).IsUser() && !isHelpOn)
					players.at(currentPlayer).SubChip();
				break;

			case ClickedButton::CASH:
				if (startBets && players.at(currentPlayer).GetChipsCount() && !isHelpOn) {//if player has a bet of chips
					players.at(currentPlayer).SetBet();//completion of finish 
					SkipPlayer();//go to next player
				}
				break;

			case ClickedButton::HELP:
				isHelpOn = !isHelpOn;
				break;

			case ClickedButton::CHANGE_SKIN:
				if(!isHelpOn)
					Card::ChangeCardSkin();

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

	if (isHelpOn) 
		DrawHelpTittles();

	else{
		//draw deck cards
		DrawDeckCard();
		
		//player cards
		for (int p = 0; p < players.size(); p++)
			players.at(p).DrawCards(render);

		//player chips
		for (int p = 0; p < players.size(); p++)
			players.at(p).DrawChips(render);

		//gui elements
		string buffCash = to_string(players.at(currentPlayer).GetCash()) + " $";
		if (players.at(currentPlayer).IsUser())
			buttons.at(cashButtonIndx).SetTittle(buffCash, render);

		for (int b = 0; b < buttons.size(); b++)
			buttons.at(b).Draw(render);

		DrawUserBetTittles();
		DrawUserTurnTittles();
		
		if(isEndGame)
			tittles[TITTLE_GAME_END_ROUND].Draw(render);
	}
		
	SDL_RenderPresent(render);
}

void Game::AutoPlay() {
	if ((!players.at(currentPlayer).IsUser() || distributionCards)//waiting when turn not user 
		&& !isEndGame) {

		playersTurnTime += deltaTime;

		if (playersTurnTime >= /*(distributionCards) ? 10.0f :*/ 40.0f) {
			playersTurnTime = 0;

			if ((!distributionCards && players.at(currentPlayer).IsDealer())) //dealer turn during notStart game OR if player stand
				SkipPlayer();

			else if (!distributionCards && !players.at(currentPlayer).IsUser())//when the handing of cards is completed(distributionCards),
				//provided that currplayer not user - turn on AI other for players
				NPCGamePlay(players.at(currentPlayer));//Take or skip

			else if (distributionCards)//auto distribution of cards to everyone
				TakeCard(players.at(currentPlayer), players.at(currentPlayer).IsDealer());
		}
	}
};

void Game::TakeCard(Player& player, bool isDealer) {
	if (!cardPlayDeck.empty()) {

		Mix_PlayChannel(-1, cardFlip_Song, 0);
		topCardIsMove = true;
		cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);

		while (isDealer && cardPlayDeck.top().GetType() == CARD_TYPE_ACE) {//if an ace, then the dealer will take the king until it is an ace
			cardPlayDeck.pop();
			cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);
		}

		(isDealer && player.GetCardsCount() == 1) ?//the dealer's second card must be closed
			cardPlayDeck.top().SetUpsideDown(true) :
			cardPlayDeck.top().SetUpsideDown(false);
		player.AddCard(cardPlayDeck.top());

		cardPlayDeck.pop();
	}
};

void Game::SkipTake() {
	players.at(currentPlayer).SetStand();
	/*if(!players.at(currentPlayer).IsBust())//if player isn`t eliminated*/
	players.at(currentPlayer).SetGameResult("STAND", render, { 255, 255, 102, 255 }, { 102, 51, 0, 255 });

	SkipPlayer();
};

void Game::SkipPlayer() {
	(currentPlayer < players.size() - 1) ?
		currentPlayer++ :
		currentPlayer = 0;
	
	if (players.at(currentPlayer).IsStand() && !isEndGame)
		SkipPlayer();
};

void Game::EndGame(){
	int playerCardsValue{ 0 },
		playerAcesCnt{ 0 },
		dealerCardsValue = players.at(0).GetCardsValue();

	players.at(0).GetLastCard().SetUpsideDown(false); //dealer card set upside down 

	if (dealerCardsValue <= 16) //if the dealer has 16 points, then he takes another card
		TakeCard(players.at(0));
	players.at(0).SetStand(); //stop the dealer

	if (dealerCardsValue > 21) {//if the dealer loses, then all players double the prize 

		PlayerBust(players.at(0));

		for (int p = 1; p < players.size(); p++)
			if (!players.at(p).IsBust())//if player not bust and dealer have a 21+ value of cards
				WinChips(players.at(p), players.at(p).GetChipsCount() * 2, true);

	}else
		for (int p = 1; p < players.size(); p++) {
			if (!players.at(p).IsBust()) {//if the players are not eliminated
				playerCardsValue = players.at(p).GetCardsValue();
				playerAcesCnt = players.at(p).GetAcesCount();

				if ((playerCardsValue + playerAcesCnt) > dealerCardsValue// and have more points than the dealer
					|| (playerCardsValue + playerAcesCnt * 11) > dealerCardsValue)

					WinChips(players.at(p), players.at(p).GetChipsCount() * 2, true);//then they get a double prize
				
				else
					PlayerBust(players.at(p));//otherwise, if there are equal or fewer points - they lose
			}
		}
};

void Game::MadeBets() {
	if (startBets) {
		if (!players.at(currentPlayer).IsUser())//if other players
			playersBetTime += deltaTime;

		if (playersBetTime >= 35.0f) {
			playersBetTime = 0;
			NPCMadeBet(players.at(currentPlayer));
			SkipPlayer(); //only for skip players Index
		}
	}
	else {
		//auto take cards
		AutoPlay();
		//animation cards
		CardAnimIntercep();
	}
}

void Game::NPCGamePlay(Player& player) {//
	int assumptionErr = 1 + rand() % 4,
		plyerCardsValue = player.GetCardsValue() + player.GetAcesCount() * 11;

	(plyerCardsValue + assumptionErr < 21) ?
		TakeCard(player) :
		SkipTake();
};

void Game::NPCMadeBet(Player& player) {
	if (player.IsDealer()) //dealer has a one universal chip
			player.AddChip(Chip(true, render), true);

	else {
		int chipsBetCnt = 1 + rand() % ((player.GetCash() - 200) / 200);//minimal bet is one chip

		for (int bets = 0; bets < chipsBetCnt; bets++) {
			player.AddChip(Chip(false, render));

			Mix_PlayChannel(-1, chip_Song, 0);
		}
	}
	
	player.SetBet();
};

void Game::DrawDeckCard() {
	if (!cardPlayDeck.empty()) {
		cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);

		for (int card = 1; card < cardPlayDeck.size(); card++) {
			(card & 1) ?
				cardPlayDeck.top().MoveToCoord(cardPlayDeck.top().GetCoordX() + 2, cardPlayDeck.top().GetCoordY() - 2) :
				cardPlayDeck.top().MoveToCoord(cardPlayDeck.top().GetCoordX(), cardPlayDeck.top().GetCoordY());
		
			cardPlayDeck.top().Draw(render);
		}
	}
};

void Game::DrawHelpTittles() {
	SDL_SetRenderDrawColor(render, 20, 10, 10, 100);
	SDL_RenderFillRect(render, NULL);

	for (int helpTittle = TITTLE_HELP; helpTittle <= TITTLE_HELP7; helpTittle++)
		tittles[helpTittle].Draw(render);

	buttons.back().Draw(render);//last button always Help
};

void Game::DrawUserBetTittles() {
	if (players.at(currentPlayer).IsUser() && startBets) {
		tittles[TITTLE_YOUR_TURN].Draw(render);
		tittles[TITTLE_MADE_BET].Draw(render);
		tittles[TITTLE_MADE_BET1].Draw(render);
	}
};

void Game::DrawUserTurnTittles() {
	if (!distributionCards && !startBets && players.at(currentPlayer).IsUser() && !players.at(currentPlayer).IsStand()) {
		tittles[TITTLE_YOUR_TURN].Draw(render);
		tittles[TITTLE_TAKE_OR_SKIP].Draw(render);
		tittles[TITTLE_TAKE_OR_SKIP1].Draw(render);
	}
};


void Game::WinChips(Player& player, const int& cntChips, const bool& isDoublePrice) {
	string winResult = (isDoublePrice) ? 
		"WIN X2" : "WIN X1.5";

	for (int c = 0; c < cntChips; c++) 
		player.AddChip(players.at(0).GetChip(), true); //-take- copy chip from the dealer and give it to player

	player.SetStand();
	player.SetBust();//after receiving the prize, the player leaves the game
	player.SetGameResult(winResult, render, { 255, 128, 0, 255 }, { 0,0,0,255 });
};

void Game::PlayerBust(Player& player) {
	string bustResult = "BUST, points: " + to_string(player.GetCardsValue());
	int playerChipsCount = player.GetChipsCount();

	if (!player.IsDealer())//dealer dont sub chip
		for (int c = 0; c < playerChipsCount; c++)
			player.SubChip(false);

	player.SetStand();
	player.SetBust();
	player.SetGameResult(bustResult, render, { 128, 255, 0, 255 }, { 0,0,0,255 });
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