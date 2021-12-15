#pragma once

#include <vector>
#include <memory>
#include <map>

#include "generics.h"

struct Graphics;
struct Sprite;
struct Rectangle;

struct Map {
	enum TileType {
		EMPTY_TILE,
		WALL_TILE,

		LEFT_TRIANGLE_TILE,
		RIGHT_TRIANGLE_TILE,

		LOW_TILE,
		MEDIUM_TILE,
		HIGH_TILE
	};
	struct CollisionTile {
		CollisionTile(std::vector<int> heights, int row, int col, TileType tile_type) : heights(heights), row(row), col(col), tile_type(tile_type) {}

		std::vector<int> heights;
		int row, col;
		TileType tile_type;
	};

	static Map* createTestMap(Graphics& graphics);

	std::vector<Map::CollisionTile> getCollidingTiles(const Rectangle& rectangle) const;
	Map::CollisionTile getCollidingTile(float x, float y) const;

	void draw(Graphics& graphics) const;
private:
	struct Tile {
		Tile() {}
		Tile(TileType tile_type, std::shared_ptr<Sprite> sprite) : tile_type(tile_type), sprite(sprite) {}

		TileType tile_type;
		std::shared_ptr<Sprite> sprite;
	};

	std::vector<std::vector<Tile>> tiles_; // эта штучка сама по себе предполагает x,y позиции - мы же обращаемся к ним как [row][col]
};