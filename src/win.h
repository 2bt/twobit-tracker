#pragma once

#include <SDL2/SDL.h>

class Win {
public:
	Win() {}
	virtual ~Win() {}
	virtual void draw() = 0;
	virtual void key(const SDL_Keysym & ks) {}
	virtual void resize() {}
};
