#pragma once

#include <memory>

#include "generics.h"

struct Graphics;
struct Player;
struct Map;
struct Camera;

struct Game {
	Game();
	~Game();

private:
	void eventLoop();
	void update(int elapsed_time);
	void draw(Graphics& graphics);

	std::shared_ptr<Map> map_;
	std::shared_ptr<Player> player_;
	std::shared_ptr<Camera> camera_;
};