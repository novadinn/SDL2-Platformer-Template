#pragma once

#include "sprite.h"
#include "generics.h"

struct Graphics;
struct Map;
struct Rectangle;

struct Player {
	Player(Graphics& graphics, float x, float y);

	void update(int elapsed_time, const Map& map);
	void draw(Graphics& graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void startJump();
	void stopJump();

	float get_center_x();
	float get_center_y();

private:
	void updateX(int elapsed_time, const Map& map);
	void updateY(int elapsed_time, const Map& map);

	Rectangle leftCollision(float delta);
	Rectangle rightCollision(float delta);
	Rectangle topCollision(float delta);
	Rectangle bottomCollision(float delta);

	bool on_ground() const { return on_ground_; }

	float x_, y_;
	float velocity_x_, velocity_y_;
	float move_direction_;

	float delta_x_; // __DEBUG__
	float delta_y_; // __DEBUG__

	bool on_ground_;
	bool jump_active_;

	Sprite sprite_;
};