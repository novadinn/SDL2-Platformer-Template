#pragma once

#include <map>
#include <string>
#include <memory>

#include "generics.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;
struct Rectangle; // __DEBUG__

struct Graphics {
	Graphics();
	~Graphics();

	SDL_Texture* loadImage(const std::string& file_name, bool black_is_transparent);

	void blitSurface(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle);

	void clear();
	void flip();

	void set_offset(float x, float y);

	void drawRect(const Rectangle& rectangle, int r, int g, int b); // __DEBUG__
private:
	std::map<std::string, SDL_Texture*> sprite_sheets_;

	SDL_Window* window_;
	SDL_Renderer* renderer_;

	float x_offset_, y_offset_;
};