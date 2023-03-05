#include "Headers/Game.h"
#include <Windows.h>


void Game::Error(const char str[]) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s\n", str);
};

SDL_Texture* Game::LoadTexture(const string& filePath, SDL_Renderer* renderTarg) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		SDL_Log(IMG_GetError());
	else {
		texture = SDL_CreateTextureFromSurface(renderTarg, surface);
		if (texture == NULL)
			SDL_Log(SDL_GetError());
	}

	SDL_FreeSurface(surface);
	return texture;
}

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

	while (!cardPlayDeck.empty())
		cardPlayDeck.pop();

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
	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);//get display params

	window = SDL_CreateWindow("BlackJack",
		0, 0,
		display.w, display.h,
		SDL_WINDOW_OPENGL);
	render = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);

	background = LoadTexture("Resource\\Images\\background.png", render);


	DeckGeneration();

	isRun = true;
};

void Game::Stop() {
	isRun = false;
};

void Game::DeckGeneration() {
	srand(time(0));
	int randIndex{ 0 };
	list<Card> DefaultDeck;
	list<Card>::iterator deckIter;

	for (int suit = CARD_SUIT_SPADE; suit <= CARD_SUIT_DIAMOND; suit++)
		for (int type = CARD_TYPE_TWO; type <= CARD_TYPE_ACE; type++)
			DefaultDeck.emplace_back(Card(suit, type));
 	
	while (cardPlayDeck.size() != 52) {//init card deck
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

	SDL_RenderCopy(render, background, NULL, NULL);

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