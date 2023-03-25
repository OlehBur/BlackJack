#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <memory>



struct SDL_WndDeleter {
	void operator()(SDL_Window* wnd) /*const*/ {
		SDL_DestroyWindow(wnd);
	}
};

struct SDL_RndrDeleter {
	void operator()(SDL_Renderer* rndr) {
		SDL_DestroyRenderer(rndr);
	}
};

struct SDL_SrfcDeleter {
	void operator()(SDL_Surface* srfc) {
		SDL_FreeSurface(srfc);
	}
};

struct SDL_FontDeleter {
	void operator()(TTF_Font* font) {
		TTF_CloseFont(font);
	}
};

struct SDL_TxtrDeleter {
	void operator()(SDL_Texture* txtr) {
		SDL_DestroyTexture(txtr);
	}
};

struct SDL_MusDeleter {
	void operator()(Mix_Music* mus) {
		Mix_FreeMusic(mus);
	}
};

struct SDL_ChunkDeleter {
	void operator()(Mix_Chunk* chunk) {
		Mix_FreeChunk(chunk);
	}
};