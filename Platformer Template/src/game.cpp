#include "game.h"

#include <SDL.h>

#include "graphics.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "camera.h"

namespace {
	const int kDeltaTime = 1;
	const int kGameSpeed = 1;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	eventLoop();
}

Game::~Game() {
	SDL_Quit();
}

void Game::eventLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	map_.reset(new Map(graphics, "map", "tile_sheet"));
	player_.reset(new Player(graphics, 320, 240));
	camera_.reset(new Camera(0, 0, generics::kScreenWidth, generics::kScreenHeight));

	int last_update_time = SDL_GetTicks();
	int frame_time_accumulator = 0;
	bool stop_running = false;
	bool running = true;
	while (running) {
		const int start_frame_time = SDL_GetTicks();
		const int frame_time = start_frame_time - last_update_time;
		last_update_time = start_frame_time;
		frame_time_accumulator += frame_time;

		while (frame_time_accumulator >= kDeltaTime) {
			input.beginNewFrame();
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (!event.key.repeat)
						input.keyDownEvent(event);
					break;
				case SDL_KEYUP:
					input.keyUpEvent(event);
					break;
				default:
					break;
				}
			}

			if (input.wasKeyPressed(SDLK_ESCAPE)) {
				running = false;
			}

			if (input.isKeyHeld(SDLK_a) && input.isKeyHeld(SDLK_d)) {
				player_->stopMoving();
			} else if (input.isKeyHeld(SDLK_a)) {
				player_->startMovingLeft();
			} else if (input.isKeyHeld(SDLK_d)) {
				player_->startMovingRight();
			} else {
				player_->stopMoving();
			}

			if (input.wasKeyPressed(SDLK_SPACE)) {
				player_->startJump();
			} else if (input.wasKeyReleased(SDLK_SPACE)) {
				player_->stopJump();
			}

			if (input.wasKeyPressed(SDLK_n))
				stop_running = !stop_running;

			if(!stop_running)
				update(kDeltaTime*kGameSpeed);

			frame_time_accumulator -= kDeltaTime;
		}

		draw(graphics);
	}
}

void Game::update(int elapsed_time) {
	player_->update(elapsed_time, *map_);
	camera_->update(player_->get_center_x(), player_->get_center_y());
}

void Game::draw(Graphics& graphics) {
	graphics.clear();

	map_->draw(graphics);
	player_->draw(graphics);
	camera_->draw(graphics);

	graphics.flip();
}
