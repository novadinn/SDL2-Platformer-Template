#include "graphics.h"

#include <SDL.h>

#include "rectangle.h" // __DEBUG__

Graphics::Graphics() {
    window_ = SDL_CreateWindow("Platformer Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, generics::kScreenWidth, generics::kScreenHeight, 0);
	renderer_ = SDL_CreateRenderer(window_, -1, 0);
    x_offset_ = 0.0f;
    y_offset_ = 0.0f;
}

Graphics::~Graphics() {
    for (std::map<std::string, SDL_Texture*>::iterator iter = sprite_sheets_.begin();
        iter != sprite_sheets_.end();
        ++iter) {
        SDL_DestroyTexture(iter->second);
    }
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

SDL_Texture* Graphics::loadImage(const std::string& file_name, bool black_is_transparent) {
	const std::string file_path = "content/" + file_name + ".bmp";
    if (sprite_sheets_.count(file_path) == 0) {
        SDL_Surface* image = SDL_LoadBMP(file_path.c_str());
        if (black_is_transparent) {
            const Uint32 black_color = SDL_MapRGB(image->format, 0, 0, 0);
            SDL_SetColorKey(image, SDL_TRUE, black_color);
        }
        sprite_sheets_[file_path] = SDL_CreateTextureFromSurface(renderer_, image);
        SDL_FreeSurface(image);
    }
    return sprite_sheets_[file_path];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle) {
    if (source_rectangle) {
        destination_rectangle->w = source_rectangle->w;
        destination_rectangle->h = source_rectangle->h;
    } else {
        uint32_t format;
        int access, w, h;
        SDL_QueryTexture(source, &format, &access, &w, &h);
        destination_rectangle->w = w;
        destination_rectangle->h = h;
    }
    destination_rectangle->x += generics::round(x_offset_);
    destination_rectangle->y += generics::round(y_offset_);

    SDL_RenderCopy(renderer_, source, source_rectangle, destination_rectangle);
}

void Graphics::clear() {
	SDL_RenderClear(renderer_);
}

void Graphics::flip() {
	SDL_RenderPresent(renderer_);
}

void Graphics::set_offset(float x, float y) {
    x_offset_ = x;
    y_offset_ = y;
}

void Graphics::drawRect(const Rectangle& rectangle, int r, int g, int b) { // __DEBUG__
    SDL_Rect rect;
    rect.x = generics::round(rectangle.left()+x_offset_);
    rect.y = generics::round(rectangle.top()+y_offset_);
    rect.w = generics::round(rectangle.width());
    rect.h = generics::round(rectangle.height());

    SDL_SetRenderDrawColor(renderer_, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer_, &rect);
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
}