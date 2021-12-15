#pragma once

#include <SDL.h>

struct Graphics;

struct Camera {

	Camera(float x, float y, float width, float height);

	void update(float player_center_x, float player_center_y);
	void draw(Graphics& graphics) const;

	float get_x() const { return rectangle_.x * 1.0f; }
	float get_y() const { return rectangle_.y * 1.0f; }

private:
	SDL_Rect rectangle_;
};