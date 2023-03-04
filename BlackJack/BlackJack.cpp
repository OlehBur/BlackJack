#include<SDL.h>

#include"Source/Headers/Game.h"
#include"Source/Headers/Card.h"


int main(int argc, char** argv)
{

	Game BlackJack;
	
	BlackJack.Init();
	while (BlackJack.IsRun()) {
		while (SDL_PollEvent(&BlackJack.GetEvent()) != 0) {
			switch (BlackJack.GetEvent().type) {
			case SDL_MOUSEBUTTONDOWN:
				BlackJack.Stop();
				break;
			}
		}

		BlackJack.Draw();
	}
	//SDL_Renderer* mainRender = nullptr;
	//SDL_Texture* texture = Game::LoadTexture("Resource/Images/Cards skins/Default/jack_of_clubs2.png", 0);

	return 0;
}
