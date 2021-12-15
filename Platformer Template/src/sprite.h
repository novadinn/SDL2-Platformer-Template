#pragma once

#include <SDL.h>
#include <string>

#include "generics.h"

struct Graphics;

struct Sprite {
	Sprite(Graphics& graphics, const std::string& file_name, int x, int y, int width, int height);

	virtual void update() {}
	void draw(Graphics& graphics, float x, float y);
protected:
	SDL_Rect source_rectangle_;
private:
	SDL_Texture* sprite_sheet_;
};