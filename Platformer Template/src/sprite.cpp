#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(Graphics& graphics, const std::string& file_name, int x, int y, int width, int height) {
	bool black_is_transparent = true;
	sprite_sheet_ = graphics.loadImage(file_name, black_is_transparent);
	source_rectangle_.x = x;
	source_rectangle_.y = y;
	source_rectangle_.w = width;
	source_rectangle_.h = height;
}

Sprite::Sprite(Graphics& graphics, const std::string& file_name) {
	bool black_is_transparent = true;
	sprite_sheet_ = graphics.loadImage(file_name, black_is_transparent);
	
	uint32_t format;
	int access, w, h;
	SDL_QueryTexture(sprite_sheet_, &format, &access, &w, &h);
	source_rectangle_.x = 0;
	source_rectangle_.y = 0;
	source_rectangle_.w = w;
	source_rectangle_.h = h;
}

void Sprite::draw(Graphics& graphics, float x, float y) {
	SDL_Rect destination_rectangle;
	destination_rectangle.x = generics::floatToInt(x);
	destination_rectangle.y = generics::floatToInt(y);
	graphics.blitSurface(sprite_sheet_, &source_rectangle_, &destination_rectangle);
}
