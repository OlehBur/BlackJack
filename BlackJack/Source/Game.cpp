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

		Mix_Init(MIX_INIT_MP3);
		Mix_OpenAudio(
			game_audio_frequency,
			MIX_DEFAULT_FORMAT,
			game_audio_channels, 
			game_audio_chunk_size/*~2kB*/); // 44100

		TTF_Init();
	}

	else {
		Error("The Game class cannot have more than one object.");
		exit(0);
	}
};

Game::~Game() {
	//for (int b = 0; b < buttons.size(); b++)
	//	buttons.at(b)./*~Button*/Destructor_Button();
	buttons.clear();

	//for (map <int, Tittle >::iterator t = tittles.begin(); t!=tittles.end(); advance(t, 1))
	//	t->second.Destructor_Tittle/*~Tittle*/();
	tittles.clear();

	for (int p = 0; p < players.size(); p++)
		players.at(p).Destructor_Player();
	players.clear();

	while (!cardPlayDeck.empty()) {
		//cardPlayDeck.top().Destructor_Card();
		cardPlayDeck.pop();
	}

	//SDL_DestroyWindow(window);
	//SDL_DestroyRenderer(render);
	//SDL_DestroyTexture(background);

	/*window = nullptr;
	render= nullptr;*/
	//background = nullptr;
	//musicBackground = nullptr;
	//cardFlip_Song = nullptr;
	//buttonClick_Song = nullptr;
	//chip_Song = nullptr;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
};


void Game::CardAnimIntercep() {
	if (topCardIsMove) {//only when cards is moving
		frameTime += deltaTime;
		if (frameTime >= card_animation_update_time) {
			frameTime = 0;
			if (players.at(currentPlayerIndex).GetLastCard().AnimateMotion(players.at(currentPlayerIndex).GetCardCoord(),
				deltaTime, render.get(), !players.at(currentPlayerIndex).GetPlacement())) {//placement return invert numb, if default(0) - 1(true)
				players.at(currentPlayerIndex).PlayerCardsCentered();
				players.at(currentPlayerIndex).CardsUpatePlacement();

				topCardIsMove = false;

				SkipPlayer();
			}
		}
	}
};


void Game::MusicInit() {
	musicBackground.reset(Mix_LoadMUS("Resource\\Audio\\Stardust.mp3"));
	cardFlip_Song.reset(Mix_LoadWAV("Resource\\Audio\\flipCard.mp3"));
	buttonClick_Song.reset(Mix_LoadWAV("Resource\\Audio\\buttonClick.wav"));
	chip_Song.reset(Mix_LoadWAV("Resource\\Audio\\chip.wav"));
};

void Game::InitPlayers() {
	players.reserve(4);

	players.push_back(Player(CARD_PLACE_DEFAULT, render.get(), false, chip_default_price, true));//dealer
	players.back().SetCardsCoord({
		display.w / 2,		//TODO: create a system for dividing the screen into sectors and use them for coords of objects
		display.h / 4 });

	players.push_back(Player(CARD_PLACE_RIGHT, render.get()));
	players.back().SetCardsCoord({ 
		display.w - display.w / 20,
		int(display.h / 1.9) });

	players.push_back(Player(CARD_PLACE_DEFAULT, render.get(), true));
	players.back().SetCardsCoord({
		display.w / 2, 
		int(display.h / 1.01) });

	players.push_back(Player(CARD_PLACE_LEFT, render.get()));
	players.back().SetCardsCoord({
		display.w / 20, 
		int(display.h / 1.9) });
};

void Game::InitTittles() {
		//turns help
	tittles.emplace(TITTLE_YOUR_TURN, Tittle());
	tittles[TITTLE_YOUR_TURN].InitFont(render.get(),
		display.w / 2,			//TODO: create a system for dividing the screen into sectors and use them for coords of objects
		display.h / 2.2, textMaxSize);
	tittles[TITTLE_YOUR_TURN].SetText("Your turn, player!", render.get());

	tittles.emplace(TITTLE_MADE_BET, Tittle());
	tittles[TITTLE_MADE_BET].InitFont(render.get(),
		display.w / 2,
		display.h / 1.8, textMaxSize * header_third_scale);
	tittles[TITTLE_MADE_BET].SetText("Place your bet by clicking on \"+\" or \"-\".", render.get());

	tittles.emplace(TITTLE_MADE_BET1, Tittle());
	tittles[TITTLE_MADE_BET1].InitFont(render.get(), 
		display.w / 2,
		display.h / 1.6, textMaxSize * header_third_scale);
	tittles[TITTLE_MADE_BET1].SetText("Then click on your cashe to confirm the bet.", render.get());

	tittles.emplace(TITTLE_TAKE_OR_SKIP, Tittle());
	tittles[TITTLE_TAKE_OR_SKIP].InitFont(render.get(),
		display.w / 2,
		display.h / 1.8, textMaxSize * header_third_scale);
	tittles[TITTLE_TAKE_OR_SKIP].SetText("You can take a card by clicking on the \"HIT\"", render.get());

	tittles.emplace(TITTLE_TAKE_OR_SKIP1, Tittle());
	tittles[TITTLE_TAKE_OR_SKIP1].InitFont(render.get(),
		display.w / 2,
		display.h / 1.6, textMaxSize * header_third_scale);
	tittles[TITTLE_TAKE_OR_SKIP1].SetText(" or miss a turn by clicking on the \"STAND\"", render.get());

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
	tittles[TITTLE_HELP].InitFont(render.get(), 
		display.w / 2,
		display.h / 3.85, textMaxSize);
	tittles[TITTLE_HELP].SetText("BLACKJACK HELPER", render.get());

	for (int helpTittle = TITTLE_HELP1, cntIndex{ 0 }; helpTittle <= TITTLE_HELP7; helpTittle++, cntIndex++) {
		tittles.emplace(helpTittle, Tittle());
		tittles[helpTittle].InitFont(render.get(),
			display.w / 2,
			display.h / 3 + (display.h / 15) * cntIndex, textMaxSize * header_fourth_scale);
		tittles[helpTittle].SetText(helpText[cntIndex], render.get());
	}
	
		//end round msg
	tittles.emplace(TITTLE_GAME_END_ROUND, Tittle());
	tittles[TITTLE_GAME_END_ROUND].InitFont(render.get(),
		display.w / 2,
		display.h / 2.1, textMaxSize);
	tittles[TITTLE_GAME_END_ROUND].SetText("- Round is complete -", render.get());

		//game end msg
	tittles.emplace(TITTLE_GAME_END, Tittle());
	tittles[TITTLE_GAME_END].InitFont(render.get(),
		display.w / 2,
		display.h / 2.1, textMaxSize);
	tittles[TITTLE_GAME_END].SetText("- Game Over -", render.get());

};

void Game::InitButtons() {
	buttons.reserve(4);

	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_HIT, textMaxSize * button_main_scale));//Hit
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.2),	//x
		int(display.h - buttons.back().GetRect().h * 2.1));	//y 

	buttons.back().SetTittle("HIT", render.get());

	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_STAND, textMaxSize * button_main_scale)); //Stand
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.2),		//TODO: create a system for dividing the screen into sectors and use them for coords of objects
		int(display.h - buttons.back().GetRect().h * 1.1));

	buttons.back().SetTittle("STAND", render.get());

	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_CASH, textMaxSize * button_main_scale)); //Cash
	buttons.back().SetCoord(
		display.w / 20,
		int(display.h - buttons.back().GetRect().h * 2.1));
	buttons.back().SetTittle("0 $", render.get());
	cashButtonIndx = buttons.size() - 1;

	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_CASH_MINUS, textMaxSize * button_addit_scale)); //minus
	buttons.back().SetCoord(
		display.w / 20,
		int(display.h - buttons.back().GetRect().h * 1.1));
	buttons.back().SetTittle("-", render.get());


	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_CASH_PLUS, textMaxSize * button_addit_scale)); //plus
	buttons.back().SetCoord(
		display.w / 20 + buttons.at(0).GetRect().w - buttons.back().GetRect().w,//0 - index of default button such as cash. align along the edge of this button
		int(display.h - buttons.back().GetRect().h * 1.1));
	buttons.back().SetTittle("+", render.get());


	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_CHANGE_SKIN, textMaxSize * button_small_scale)); //change skin
	buttons.back().SetCoord(
		int(buttons.back().GetRect().w / 2 * .3),
		display.h / 15);
	buttons.back().SetTittle("Change Card Skin", render.get());


	buttons.emplace_back(Button(render.get(), BUTTON_TYPE_HELP, textMaxSize * button_big_scale)); //help
	buttons.back().SetCoord(
		int(display.w - buttons.back().GetRect().w * 1.1),
		display.h / 15);
	buttons.back().SetTittle("?", render.get());
};


void Game::Init() {
	SDL_GetCurrentDisplayMode(0, &display);//get display params
	DECK_POS_X = display.w - display.w / 5;
	DECK_POS_Y = display.h / 5;

	textMaxSize = display.h * tittle_main_scale;

	window.reset(SDL_CreateWindow("BlackJack",
		0, 0,
		display.w, display.h,
		SDL_WINDOW_OPENGL));

	render.reset(SDL_CreateRenderer(window.get(),
		-1, SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC));

	background.reset(GameItems::LoadTexture("Resource\\Images\\GUI\\background.png", render.get()));

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
		playerAcesCnt{ 0 },
		playersBetCnt{ 0 },
		playersStandCnt{ 0 };

	prevTime = currentTime;
	currentTime = SDL_GetTicks();
	deltaTime = (currentTime - prevTime) / deltaTime_koefficient;

	if (!Mix_PlayingMusic() && isMusicOn)
		Mix_PlayMusic(musicBackground.get(), -1);//2 param - cnt of rep, if -1 wil rep until don`t say stop

	MadeBets();

	if (players.at(0).GetCardsCount() >= dealer_default_cardsCnt) //if dealer has two cards
		distributionCards = false;

	if (!isEndRound) {
		for (int p = 1; p < players.size(); p++) {
			playerCardsValue = players.at(p).GetCardsValue();
			playerAcesCnt = players.at(p).GetAcesCount();

			if ((playerCardsValue + playerAcesCnt == winning_cards_value //if player has a 21 points
				|| playerCardsValue + playerAcesCnt * ace_max_value == winning_cards_value)
				&& !players.at(p).IsBust())//if player not eliminated

				WinChips(players.at(p), players.at(p).GetChipsCount() * oneAndHalf_prize_factor, false);// add win chips x1.5 to player if he has a 21 points

			else if (players.at(p).GetCardsValue() > winning_cards_value 
				&& !players.at(p).IsBust())

				PlayerBust(players.at(p));
		}
	}

	for (int p = 0; p < players.size(); p++) {
		(players.at(p).IsMadeBet()) ?
			playersBetCnt++ : playersBetCnt = 0;

		(players.at(p).IsStand()) ?
			playersStandCnt++ : playersStandCnt = 0;
	}

	if (playersBetCnt == players.size() && startBets) {//checking if everyone has bid
		startBets = false;
		distributionCards = true;
	}
	

	if (playersStandCnt >= players.size() - 1
		&& !isEndGame) {//checking if everyone clicked the stand except the dealer

		EndRound();
		isEndRound = true;
		NextRound();
	}

	if (cardPlayDeck.size() < min_allow_numb_cards && isEndRound)
		isEndGame = true;
};

void Game::MouseActivity(const SDL_Rect& mousePos, const bool& isClick) {

	for (int b = 0; b < buttons.size(); b++)
		if (buttons.at(b).Interact(mousePos, isClick)) {
			
			if(!isHelpOn)
				Mix_PlayChannel(-1, buttonClick_Song.get(), 0);

			switch (Button::currentButtonClicked) {
			case ClickedButton::HIT:
				if(!topCardIsMove 
					&& players.at(currentPlayerIndex).IsUser()
					&& !players.at(currentPlayerIndex).IsStand()
					&& !distributionCards 
					&& !startBets && !isHelpOn)
				TakeCard(players.at(currentPlayerIndex));//always user iter
				break;

			case ClickedButton::STAND:
				if (!topCardIsMove
					&& players.at(currentPlayerIndex).IsUser()
					&& !players.at(currentPlayerIndex).IsStand()
					&& !distributionCards
					&& !startBets && !isHelpOn)
					
					SkipTake();
				break;

			case ClickedButton::CASH_PLUS:
				if(startBets && players.at(currentPlayerIndex).IsUser() && !isHelpOn)
					players.at(currentPlayerIndex).AddChip(Chip{ false, render.get() });
				break;

			case ClickedButton::CASH_MINUS:
				if (startBets && players.at(currentPlayerIndex).IsUser() && !isHelpOn)
					players.at(currentPlayerIndex).SubChip();
				break;

			case ClickedButton::CASH:
				if (startBets && players.at(currentPlayerIndex).GetChipsCount() && !isHelpOn) {//if player has a bet of chips
					players.at(currentPlayerIndex).SetBet(true);//completion of finish 
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
			DefaultDeck.emplace_back(Card(suit, type, render.get()));
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

	SDL_RenderClear(render.get());

	//draw Backgr
	SDL_RenderCopy(render.get(), background.get(), NULL, NULL);

	if (isHelpOn) 
		DrawHelpTittles();

	else{
		//Init`s
		//InitButtons();


		//draw deck cards
		DrawDeckCard();
		
		//player chips
		for (int p = 0; p < players.size(); p++)
			players.at(p).DrawChips(render.get());

		//player cards
		for (int p = 0; p < players.size(); p++)
			players.at(p).DrawCards(render.get());

		//gui elements
		string buffCash = to_string(players.at(currentPlayerIndex).GetCash()) + " $";
		if (players.at(currentPlayerIndex).IsUser())
			buttons.at(cashButtonIndx).SetTittle(buffCash, render.get());

		for (int b = 0; b < buttons.size(); b++)
			buttons.at(b).Draw(render.get());

		DrawUserBetTittles();
		DrawUserTurnTittles();
		
		if(isEndRound)
			tittles[TITTLE_GAME_END_ROUND].Draw(render.get());

		if(isEndGame)
			tittles[TITTLE_GAME_END].Draw(render.get());
	}
		
	SDL_RenderPresent(render.get());
}


void Game::NextRound() {

	if(isEndRound)
		endRoundTime += deltaTime;

	if (endRoundTime >= endRound_waiting_time) {
		endRoundTime = 0;
		isEndRound = false;

		for (int i = 0, playerCash{ 0 }; i < players.size(); i++) {
			players.at(i).DiscardCards();
			players.at(i).SetBust(false);
			players.at(i).SetBet(false);
			players.at(i).SetStand(false);
			players.at(i).RemoveTittle();

			if (players.at(i).IsDealer()) //isn`t dealer
				continue;

			players.at(i).ConvertChipToCash();
			playerCash = players.at(i).GetCash();

			if (playerCash < chip_default_price && players.at(i).IsUser()) {
				isEndGame = true;
				return;
			}
			if (playerCash < chip_default_price)//the player leaves the game because he is unable to place bets
				players.at(i).SetGameResult("Left the game", render.get(), { 255,0,0 }, { 0,0,0 });
		}

		startBets = true;
	}
};

void Game::AutoPlay() {
	if ((!players.at(currentPlayerIndex).IsUser() || distributionCards)//waiting when turn not user 
		&& !isEndRound && !isEndGame) {

		if(!topCardIsMove)
			playersTurnTime += deltaTime;

		/*auto waiting_time{
			[&] ()->float {
				if (distributionCards)
					return distributionCards_waiting_time;
				else
					return autoPlay_waiting_time;}
		};*/
		if (playersTurnTime >= Waiting_Time(distributionCards, distributionCards_waiting_time, autoPlay_waiting_time)) {
			playersTurnTime = 0;

			if ((!distributionCards && players.at(currentPlayerIndex).IsDealer())) //dealer turn during notStart game OR if player stand
				SkipPlayer();

			else if (!distributionCards && !players.at(currentPlayerIndex).IsUser())//when the handing of cards is completed(distributionCards),
				//provided that currplayer not user - turn on AI other for players
				NPCGamePlay(players.at(currentPlayerIndex));//Take or skip

			else if (distributionCards)//auto distribution of cards to everyone
				TakeCard(players.at(currentPlayerIndex), players.at(currentPlayerIndex).IsDealer());
		}
	}
};

void Game::TakeCard(Player& player, bool isDealer) {
	if (!cardPlayDeck.empty()) {

		Mix_PlayChannel(-1, cardFlip_Song.get(), 0);

		topCardIsMove = true;

		while (isDealer && cardPlayDeck.top().GetType() == CARD_TYPE_ACE) //if an ace, then the dealer will take the king until it is an ace
			cardPlayDeck.pop();

		cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);
		(isDealer && player.GetCardsCount() == 1) ?//the dealer's second card must be closed
			cardPlayDeck.top().SetUpsideDown(true) :
			cardPlayDeck.top().SetUpsideDown(false);
		player.AddCard(cardPlayDeck.top());

		cardPlayDeck.pop();
	}
};

void Game::SkipTake() {
	players.at(currentPlayerIndex).SetStand(true);

	players.at(currentPlayerIndex).SetGameResult("STAND", 
		render.get(),
		player_stand_text_color, 
		player_stand_outline_color);

	SkipPlayer();
};

void Game::SkipPlayer() {
	(currentPlayerIndex < players.size() - 1) ?
		currentPlayerIndex++ :
		currentPlayerIndex = 0;
	
	if (players.at(currentPlayerIndex).IsStand() && !isEndRound)
		SkipPlayer();
};

void Game::EndRound(){
	int playerCardsValue{ 0 },
		playerAcesCnt{ 0 },
		dealerCardsValue = players.at(0).GetCardsValue();

	players.at(0).GetLastCard().SetUpsideDown(false); //dealer card set upside down 

	if (dealerCardsValue <= norm_dealer_card_values) //if the dealer has 16 points, then he takes another card
		TakeCard(players.at(0));
	players.at(0).SetStand(true); //stop the dealer

	if (dealerCardsValue > winning_cards_value) {//if the dealer loses, then all players double the prize 

		PlayerBust(players.at(0));

		for (int p = 1; p < players.size(); p++)
			if (!players.at(p).IsBust())//if player not bust and dealer have a 21+ value of cards
				WinChips(players.at(p), players.at(p).GetChipsCount() * double_prize_factor, true);

	}else
		for (int p = 1; p < players.size(); p++) {
			if (!players.at(p).IsBust()) {//if the players are not eliminated
				playerCardsValue = players.at(p).GetCardsValue();
				playerAcesCnt = players.at(p).GetAcesCount();

				if ((playerCardsValue + playerAcesCnt) > dealerCardsValue// and have more points than the dealer
					|| (playerCardsValue + playerAcesCnt * ace_max_value) > dealerCardsValue)

					WinChips(players.at(p), players.at(p).GetChipsCount() * double_prize_factor, true);//then they get a double prize
				
				else
					PlayerBust(players.at(p));//otherwise, if there are equal or fewer points - they lose
			}
		}
};

void Game::MadeBets() {
	if (startBets) {
		if (!players.at(currentPlayerIndex).IsUser())//if other players
			playersBetTime += deltaTime;

		if (playersBetTime >= made_bet_waiting_time) {
			playersBetTime = 0;
			NPCMadeBet(players.at(currentPlayerIndex));
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

void Game::NPCGamePlay(Player& player) {
	int assumptionErr = 1 + rand() % 4,//player's guess as to value of next card
		plyerCardsValue = player.GetCardsValue() + player.GetAcesCount() * ace_max_value;

	(plyerCardsValue + assumptionErr < winning_cards_value) ?
		TakeCard(player) :
		SkipTake();
};

void Game::NPCMadeBet(Player& player) {
	if (player.IsDealer()) //dealer has a one universal chip
		player.AddChip(Chip{ true, render.get() }, true);

	else {
		int playerCurrentCash = player.GetCash();
		int chipsBetCnt = (playerCurrentCash == chip_default_price) ?
			1 : 
			1 + rand() % ((playerCurrentCash - chip_default_price) / chip_default_price);//minimal bet is one chip

		for (int bets = 0; bets < chipsBetCnt; bets++) {
			player.AddChip(Chip{ false, render.get() });

			Mix_PlayChannel(-1, chip_Song.get(), 0);
		}
	}
	
	player.SetBet(true);
};

void Game::DrawDeckCard() {
	if (!cardPlayDeck.empty()) {
		cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);

		for (int card = 1; card < cardPlayDeck.size(); card++) {
			(card & 1) ?//card even index check
				cardPlayDeck.top().MoveToCoord(
					cardPlayDeck.top().GetCoordX() + cardDeck_pos_deviation,
					cardPlayDeck.top().GetCoordY() - cardDeck_pos_deviation) :

				cardPlayDeck.top().MoveToCoord(
					cardPlayDeck.top().GetCoordX(),
					cardPlayDeck.top().GetCoordY());
		
			cardPlayDeck.top().Draw(render.get());
		}
	}
};

void Game::DrawHelpTittles() {
	SDL_SetRenderDrawColor(render.get(), 
		background_helpWnd_color.r, 
		background_helpWnd_color.b, 
		background_helpWnd_color.g,
		background_helpWnd_color.a);

	SDL_RenderFillRect(render.get(), NULL);

	for (int helpTittle = TITTLE_HELP; helpTittle <= TITTLE_HELP7; helpTittle++)
		tittles[helpTittle].Draw(render.get());

	buttons.back().Draw(render.get());//last button always Help
};

void Game::DrawUserBetTittles() {
	if (players.at(currentPlayerIndex).IsUser() && startBets) {
		tittles[TITTLE_YOUR_TURN].Draw(render.get());
		tittles[TITTLE_MADE_BET].Draw(render.get());
		tittles[TITTLE_MADE_BET1].Draw(render.get());
	}
};

void Game::DrawUserTurnTittles() {
	if (!distributionCards && !startBets && players.at(currentPlayerIndex).IsUser() 
		&& !players.at(currentPlayerIndex).IsStand()) {

		tittles[TITTLE_YOUR_TURN].Draw(render.get());
		tittles[TITTLE_TAKE_OR_SKIP].Draw(render.get());
		tittles[TITTLE_TAKE_OR_SKIP1].Draw(render.get());
	}
};


void Game::WinChips(Player& player, const int& cntChips, const bool& isDoublePrice) {
	string winResult = (isDoublePrice) ? 
		"WIN X2" : "WIN X1.5";

	for (int c = 0; c < cntChips; c++) 
		player.AddChip(Chip{ true, render.get() }/*players.at(0).GetChip()*/, true); //-take- copy chip from the dealer and give it to player

	player.SetStand(true);
	player.SetBust(true);//after receiving the prize, the player leaves the game
	player.SetGameResult(winResult, render.get(), player_win_text_color, player_win_outline_color);
};

void Game::PlayerBust(Player& player) {
	string bustResult = "BUST, points: " + to_string(player.GetCardsValue());
	int playerChipsCount = player.GetChipsCount();

	if (!player.IsDealer())//dealer dont sub chip
		for (int c = 0; c < playerChipsCount; c++)
			player.SubChip(false);

	player.SetStand(true);
	player.SetBust(true);
	player.SetGameResult(bustResult, render.get(), player_bust_text_color, player_bust_outline_color);
};

bool& Game::IsRun() {
	return isRun;
};

SDL_Event& Game::GetEvent() {
	return event;
};

SDL_Renderer* Game::GetRender() {
	return render.get();
};

float& Game::GetDeltaTime() {
	return deltaTime;
};