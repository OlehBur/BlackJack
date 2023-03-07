﻿#include <SDL.h>

#include "Source/Headers/Game.h"
#include "Source/Headers/Card.h"


int main(int argc, char** argv)
{
	int mouseX, mouseY;
	Game BlackJack;
	
	BlackJack.Init();
	while (BlackJack.IsRun()) {
		while (SDL_PollEvent(&BlackJack.GetEvent()) != 0) {

			switch (BlackJack.GetEvent().type) {
			case SDL_KEYDOWN:
				if(BlackJack.GetEvent().key.keysym.sym==SDLK_ESCAPE)
					BlackJack.Stop();
				break;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouseX, &mouseY);
				BlackJack.MouseActivity({ mouseX,mouseY,mouseX,mouseY }, false);
				break;

			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouseX, &mouseY);
				BlackJack.MouseActivity({ mouseX,mouseY,mouseX,mouseY }, true);
				break;

			case SDL_MOUSEBUTTONUP:
				SDL_GetMouseState(&mouseX, &mouseY);
				BlackJack.MouseActivity({mouseX,mouseY,mouseX,mouseY}, false);
				break;
			}
		}

		BlackJack.Draw();
	}

	return 0;
}
