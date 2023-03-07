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
	}
	else {
		Error("The Game class cannot have more than one object.");
		exit(0);
	}
};

Game::~Game() {

	buttons.clear();

	//while (!cardPlayDeck.empty())
	//	cardPlayDeck.pop();
	////cardPlayDeck.clear();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	SDL_DestroyTexture(background);

	window = nullptr;
	render = nullptr;
	background = nullptr;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
};

void Game::Init() {
	SDL_GetCurrentDisplayMode(0, &display);//get display params
	DECK_POS_X = display.w - display.w / 5;
	DECK_POS_Y = display.h / 8;

	window = SDL_CreateWindow("BlackJack",
		0, 0,
		display.w, display.h,
		SDL_WINDOW_OPENGL);
	render = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);

	background = GameItems::LoadTexture("Resource\\Images\\GUI\\background.png", render);


	DeckGeneration();

	buttons.reserve(4);

	buttons.emplace_back(Button(render, "HIT"));//Hit
	buttons.back().SetCoord({
		int(display.w - buttons.back().GetRect().w /** 1.2*/),	//x
		int(display.h - buttons.back().GetRect().h /** 1.3*/),	//y 
		0, 0 });

	buttons.emplace_back(Button(render, "STAND" )); //Stand
	buttons.back().SetCoord({
		int(buttons.back().GetRect().w /** 1.2*/- buttons.back().GetRect().w),	//x
		int(display.h - buttons.back().GetRect().h /** 1.3*/),	//y 
		0, 0 });
	

	isRun = true;
};

void Game::Stop() {
	isRun = false;
};

void Game::Update() {

};

void Game::MouseActivity(const SDL_Rect& mousePos, const bool& isClick) {
	if (isClick && cardPlayDeck.size()) {
		cardPlayDeck.pop();
		cardPlayDeck.top().InitTexture(render);
		cardPlayDeck.top().SetScaleTexture(display.w);
	}
	for (int b = 0; b < buttons.size(); b++)
		buttons.at(b).Update(mousePos, isClick);
	Update();
};

void Game::DeckGeneration() {
	srand(time(0));
	int randIndex{ 0 };
	list<Card> DefaultDeck;
	list<Card>::iterator deckIter;

	for (int suit = CARD_SUIT_SPADE; suit <= CARD_SUIT_DIAMOND; suit++)
		for (int type = CARD_TYPE_TWO; type <= CARD_TYPE_ACE; type++)
			DefaultDeck.emplace_back(Card(suit, type/*, DECK_POS_X+1, DECK_POS_Y-1*/));
 	
	//cardPlayDeck.reserve(52);
	while (cardPlayDeck.size() != 52) {//init card deck
		randIndex = (DefaultDeck.size() - 1) ?
			rand() % (DefaultDeck.size() - 1) : 0;
		deckIter = DefaultDeck.begin();
		advance(deckIter, randIndex);

		cardPlayDeck.push(*deckIter);
		//cardPlayDeck.emplace_back(*deckIter);

		DefaultDeck.erase(deckIter);//del its card from list
	}

	cardPlayDeck.top().InitTexture(render);
	cardPlayDeck.top().SetScaleTexture(display.w);

	DefaultDeck.clear();
};


void Game::Draw() {

	SDL_RenderClear(render);
	
	//draw Backgr
	SDL_RenderCopy(render, background, NULL, NULL);
	
	//draw deck
	if (cardPlayDeck.size()) {
		//int cardDeckHeight = (cardPlayDeck.size() > 25) ?
		//	25 : cardPlayDeck.size();
		cardPlayDeck.top().MoveToCoord(DECK_POS_X, DECK_POS_Y);
		for (int card = 0; card < cardPlayDeck.size(); card++) {
			(card&1)?
				cardPlayDeck.top().MoveToCoord(cardPlayDeck.top().GetCoordX() + 2, cardPlayDeck.top().GetCoordY() - 2):
				cardPlayDeck.top().MoveToCoord(cardPlayDeck.top().GetCoordX(), cardPlayDeck.top().GetCoordY());
			cardPlayDeck.top().Draw(render);
		}
	}

	//gui elements
	for (int b = 0; b < buttons.size(); b++)
		buttons.at(b).Draw(render);

	SDL_RenderPresent(render);
}

bool& Game::IsRun() {
	return isRun;
};

SDL_Event& Game::GetEvent() {
	return event;
};

SDL_Renderer* Game::GetRender() {
	return render;
};