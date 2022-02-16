#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "generics.h"

struct Graphics;
struct Sprite;
struct Rectangle;

struct Map {
	Map(Graphics& graphics, const std::string& map, const std::string& tile_sheet);

	struct CollisionTile {
		CollisionTile(std::vector<int> heights, int row, int col, int tile_type) : heights(heights), row(row), col(col), tile_type(tile_type) {}

		std::vector<int> heights;
		int row, col;
		int tile_type;
	};

	int tile_size() const { return tile_size_; }
	int empty_index() const { return empty_tile_index_; }

	float convertTileToFloat(int tile) const {
		return tile * tile_size_ * 1.0f;
	}

	int convertFloatToTile(float number) const {
		return (int)(number / tile_size_);
	}

	std::vector<Map::CollisionTile> getCollidingTiles(const Rectangle& rectangle) const;
	Map::CollisionTile getCollidingTile(float x, float y) const;

	void draw(Graphics& graphics) const;
private:

	std::shared_ptr<Sprite> map_sprite_;
	std::vector<std::vector<int>> map_array_;
	std::map<int, std::vector<int>> tile_sheet_heights_;

	int tile_size_;
	int empty_tile_index_;
};