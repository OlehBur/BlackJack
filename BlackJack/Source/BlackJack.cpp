#include<SDL.h>

#include"Headers/Game.h"
#include"Headers/Card.h"


int main(int argc, char** argv)
{
	SDL_Renderer* mainRender = nullptr;
	SDL_Texture* texture = Game::LoadTexture("Resource\Images\Cards skins\Default\jack_of_clubs2.png", mainRender);

	return 0;
}
