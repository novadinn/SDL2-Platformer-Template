#include "player.h"

#include <cmath>
#include <vector>

#include "graphics.h" // __DEBUG__
#include "map.h"
#include "rectangle.h"

namespace {
	const float kWalkingAcceleration = 0.00083007812f;
	const float kMaxSpeedX = 0.15859375f;
	const float kFriction = 0.75f;

	const float kAirAcceleration = 0.0003125f;
	const float kJumpGravity = 0.0003125f;
	const float kJumpSpeed = 0.25f;
	const float kGravity = 0.00078125f;
	const float kMaxSpeedY = 1;

	const int kSourceX = 0;
	const int kSourceY = 0;
	const int kSourceWidth = 24;
	const int kSourceHeight = 48;

	const Rectangle kCollisionRectangleX(0, 1, kSourceWidth, kSourceHeight);
	const Rectangle kCollisionRectangleY(1, 0, kSourceWidth-2, kSourceHeight);
}

Player::Player(Graphics& graphics, float x, float y) :
	x_(x), y_(y), velocity_x_(0.0f), velocity_y_(0.0f), move_direction_(0.0f),
	on_ground_(false), jump_active_(false),
	sprite_(graphics, "player", kSourceX, kSourceY, kSourceWidth, kSourceHeight)
{
	delta_x_ = 0.0f;
	delta_y_ = 0.0f;
}

void Player::update(int elapsed_time, const Map& map) {
	sprite_.update();

	updateX(elapsed_time, map);
	updateY(elapsed_time, map);
}

void Player::draw(Graphics& graphics) {
	sprite_.draw(graphics, x_, y_);

	if (delta_x_ > 0.0f) {
		graphics.drawRect(rightCollision(delta_x_), 0, 255, 0);
	} else if(delta_x_ < 0.0f) {
		graphics.drawRect(leftCollision(delta_x_), 255, 0, 0);
	}
	if (delta_y_ > 0.0f) {
		graphics.drawRect(bottomCollision(delta_y_), 0, 0, 255);
	} else if (delta_y_ < 0.0f) {
		graphics.drawRect(topCollision(delta_y_), 255, 255, 255);
	}
}

void Player::startMovingLeft() {
	move_direction_ = -1.0f;
}

void Player::startMovingRight() {
	move_direction_ = 1.0f;
}

void Player::stopMoving() {
	move_direction_ = 0.0f;
}

void Player::startJump() {
	if (on_ground()) {
		velocity_y_ = -kJumpSpeed;
	}
	jump_active_ = true;
}

void Player::stopJump() {
	jump_active_ = false;
}

float Player::get_center_x() {
	return x_ + kCollisionRectangleX.width()/2;
}

float Player::get_center_y() {
	return y_ + kCollisionRectangleY.height()/2;
}

void Player::updateX(int elapsed_time, const Map& map) {
	const float acceleration_x = on_ground() ? kWalkingAcceleration : kAirAcceleration;
	velocity_x_ += acceleration_x * move_direction_ * elapsed_time;
	if (move_direction_ < 0.0f) {
		velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
	} else if (move_direction_ > 0.0f) {
		velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
	} else if (on_ground()) {
		velocity_x_ *= kFriction;
	}

	const float delta_x = velocity_x_ * elapsed_time;
	delta_x_ = delta_x;

	if (delta_x > 0) {
		const Rectangle collision_rectangle = rightCollision(delta_x);
		float closest_pixel_position = collision_rectangle.right();
		const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
		for (size_t i = 0; i < tiles.size(); ++i) {
			if (tiles[i].tile_type != Map::EMPTY_TILE) {
				for (int j = 0; j < generics::kTileSize; ++j) {
					Rectangle height_rect(
						generics::convertTileToFloat(tiles[i].col) + j,
						generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
						1,
						tiles[i].heights[j] * 1.0f);
					if (collision_rectangle.collidesWith(height_rect)) {
						const float new_pixel_position = height_rect.left();
						if (new_pixel_position < closest_pixel_position) {
							closest_pixel_position = new_pixel_position;
						}
					}
				}
			}
		}

		if (closest_pixel_position != collision_rectangle.right()) {
			x_ = closest_pixel_position - kCollisionRectangleX.right();
			velocity_x_ = 0.0f;
		} else {
			x_ += delta_x;
		}

		{
			const Rectangle collision_rectangle = leftCollision(0);
			float closest_pixel_position = collision_rectangle.left();
			const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
			for (size_t i = 0; i < tiles.size(); ++i) {
				if (tiles[i].tile_type != Map::EMPTY_TILE) {
					for (int j = 0; j < generics::kTileSize; ++j) {
						Rectangle height_rect(
							generics::convertTileToFloat(tiles[i].col) + j,
							generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
							1,
							tiles[i].heights[j] * 1.0f);
						if (collision_rectangle.collidesWith(height_rect)) {
							const float new_pixel_position = height_rect.right();
							if (new_pixel_position > closest_pixel_position) {
								closest_pixel_position = new_pixel_position;
							}
						}
					}
				}
			}

			if (closest_pixel_position != collision_rectangle.left()) {
				x_ = closest_pixel_position - kCollisionRectangleX.left();
				velocity_x_ = 0.0f;
			}
		}
	} else {
		const Rectangle collision_rectangle = leftCollision(delta_x);
		float closest_pixel_position = collision_rectangle.left();
		const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
		for (size_t i = 0; i < tiles.size(); ++i) {
			if (tiles[i].tile_type != Map::EMPTY_TILE) {
				for (int j = 0; j < generics::kTileSize; ++j) {
					Rectangle height_rect(
						generics::convertTileToFloat(tiles[i].col) + j,
						generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
						1,
						tiles[i].heights[j] * 1.0f);
					if (collision_rectangle.collidesWith(height_rect)) {
						const float new_pixel_position = height_rect.right();
						if (new_pixel_position > closest_pixel_position) {
							closest_pixel_position = new_pixel_position;
						}
					}
				}
			}
		}

		if (closest_pixel_position != collision_rectangle.left()) {
			x_ = closest_pixel_position - kCollisionRectangleX.left();
			velocity_x_ = 0.0f;
		} else {
			x_ += delta_x;
		}

		{
			const Rectangle collision_rectangle = rightCollision(delta_x);
			float closest_pixel_position = collision_rectangle.right();
			const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
			for (size_t i = 0; i < tiles.size(); ++i) {
				if (tiles[i].tile_type != Map::EMPTY_TILE) {
					for (int j = 0; j < generics::kTileSize; ++j) {
						Rectangle height_rect(
							generics::convertTileToFloat(tiles[i].col) + j,
							generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
							1,
							tiles[i].heights[j] * 1.0f);
						if (collision_rectangle.collidesWith(height_rect)) {
							const float new_pixel_position = height_rect.left();
							if (new_pixel_position < closest_pixel_position) {
								closest_pixel_position = new_pixel_position;
							}
						}
					}
				}
			}

			if (closest_pixel_position != collision_rectangle.right()) {
				x_ = closest_pixel_position - kCollisionRectangleX.right();
				velocity_x_ = 0.0f;
			}
		}
	}
}

void Player::updateY(int elapsed_time, const Map& map) {
	const float gravity = jump_active_ && velocity_y_ < 0.0f ? kJumpGravity : kGravity;
	velocity_y_ = std::min(velocity_y_ + gravity * elapsed_time, kMaxSpeedY);

	const float delta_y = velocity_y_ * elapsed_time;
	delta_y_ = delta_y;

	if (delta_y > 0) {
		const Rectangle collision_rectangle = bottomCollision(delta_y);
		const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
		float closest_pixel_position = collision_rectangle.bottom();
		for (size_t i = 0; i < tiles.size(); ++i) {
			if (tiles[i].tile_type != Map::EMPTY_TILE) {
				for (int j = 0; j < generics::kTileSize; ++j) {
					Rectangle height_rect(
						generics::convertTileToFloat(tiles[i].col) + j,
						generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
						1,
						tiles[i].heights[j] * 1.0f);
					if (collision_rectangle.collidesWith(height_rect)) {
						const float new_pixel_position = height_rect.top();
						if (new_pixel_position < closest_pixel_position) {
							closest_pixel_position = new_pixel_position;
						}
					}
				}
			}
		}

		if (closest_pixel_position != collision_rectangle.bottom()) {
			y_ = closest_pixel_position - kCollisionRectangleY.bottom();
			on_ground_ = true;
			velocity_y_ = 0.0f;
		} else {
			if (on_ground()) {
				const float x_point = delta_x_ > 0.0f ? collision_rectangle.left() : collision_rectangle.right();
				const float y_point = collision_rectangle.bottom() + delta_y;
				Map::CollisionTile tile(map.getCollidingTile(x_point, y_point).tile_type == Map::EMPTY_TILE ?
					map.getCollidingTile(x_point, y_point + generics::kTileOffset) : map.getCollidingTile(x_point, y_point));
				if (tile.tile_type != Map::EMPTY_TILE) {
					const int height_value = (int)(x_point - generics::convertTileToFloat(tile.col));
					const float tile_height = generics::convertTileToFloat(tile.row) + generics::kTileSize - tile.heights[height_value];
					y_ = tile_height - kCollisionRectangleY.bottom();
				} else {
					y_ += delta_y;
					on_ground_ = false;
				}
			} else {
				y_ += delta_y;
				on_ground_ = false;
			}
		}

		{
			const Rectangle collision_rectangle = topCollision(0);
			float closest_pixel_position = collision_rectangle.top();
			const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
			for (size_t i = 0; i < tiles.size(); ++i) {
				if (tiles[i].tile_type != Map::EMPTY_TILE) {
					for (int j = 0; j < generics::kTileSize; ++j) {
						Rectangle height_rect(
							generics::convertTileToFloat(tiles[i].col) + j,
							generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
							1,
							tiles[i].heights[j] * 1.0f);
						if (collision_rectangle.collidesWith(height_rect)) {
							const float new_pixel_position = height_rect.bottom();
							if (new_pixel_position > closest_pixel_position) {
								closest_pixel_position = new_pixel_position;
							}
						}
					}
				}
			}

			if (closest_pixel_position != collision_rectangle.top()) {
				y_ = closest_pixel_position - kCollisionRectangleY.top();
				velocity_y_ = 0.0f;
			}
		}
	} else {
		const Rectangle collision_rectangle = topCollision(delta_y);
		float closest_pixel_position = collision_rectangle.top();
		const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
		for (size_t i = 0; i < tiles.size(); ++i) {
			if (tiles[i].tile_type != Map::EMPTY_TILE) {
				for (int j = 0; j < generics::kTileSize; ++j) {
					Rectangle height_rect(
						generics::convertTileToFloat(tiles[i].col) + j,
						generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
						1,
						tiles[i].heights[j] * 1.0f);
					if (collision_rectangle.collidesWith(height_rect)) {
						const float new_pixel_position = height_rect.bottom();
						if (new_pixel_position > closest_pixel_position) {
							closest_pixel_position = new_pixel_position;
						}
					}
				}
			}
		}

		if (closest_pixel_position != collision_rectangle.top()) {
			y_ = closest_pixel_position - kCollisionRectangleY.top();
			velocity_y_ = 0.0f;
		} else {
			y_ += delta_y;
			on_ground_ = false;
		}

		{
			const Rectangle collision_rectangle = bottomCollision(0);
			const std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(collision_rectangle));
			float closest_pixel_position = collision_rectangle.bottom();
			for (size_t i = 0; i < tiles.size(); ++i) {
				if (tiles[i].tile_type != Map::EMPTY_TILE) {
					for (int j = 0; j < generics::kTileSize; ++j) {
						Rectangle height_rect(
							generics::convertTileToFloat(tiles[i].col) + j,
							generics::convertTileToFloat(tiles[i].row) + generics::kTileSize - tiles[i].heights[j],
							1,
							tiles[i].heights[j] * 1.0f);
						if (collision_rectangle.collidesWith(height_rect)) {
							const float new_pixel_position = height_rect.top();
							if (new_pixel_position < closest_pixel_position) {
								closest_pixel_position = new_pixel_position;
							}
						}
					}
				}
			}

			if (closest_pixel_position != collision_rectangle.bottom()) {
				y_ = closest_pixel_position - kCollisionRectangleY.bottom();
				on_ground_ = true;
				velocity_y_ = 0.0f;
			}
		}
	}
}

Rectangle Player::leftCollision(float delta) {
	return Rectangle(
		x_ + kCollisionRectangleX.left() + delta,
		y_ + kCollisionRectangleX.top(),
		kCollisionRectangleX.width()/2 - delta,
		kCollisionRectangleX.height() - (on_ground() ? generics::kTileSize : generics::kTileOffset)
	);
}

Rectangle Player::rightCollision(float delta) {
	return Rectangle(
		x_ + + kCollisionRectangleX.left() + kCollisionRectangleX.width()/2,
		y_ + kCollisionRectangleX.top(),
		kCollisionRectangleX.width() / 2 + delta,
		kCollisionRectangleX.height() - (on_ground() ? generics::kTileSize : generics::kTileOffset)
	);
}

Rectangle Player::topCollision(float delta) {
	return Rectangle(
		x_ + kCollisionRectangleY.left(),
		y_ + kCollisionRectangleY.top() + delta,
		kCollisionRectangleY.width(),
		kCollisionRectangleY.height()/2 - delta
	);
}

Rectangle Player::bottomCollision(float delta) {
	return Rectangle(
		x_ + kCollisionRectangleY.left(),
		y_ + + kCollisionRectangleY.top() + kCollisionRectangleY.height()/2,
		kCollisionRectangleY.width(),
		kCollisionRectangleY.height()/2 + delta
	);
}
