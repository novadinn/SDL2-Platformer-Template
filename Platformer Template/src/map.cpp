#include "map.h"

#include <SDL.h>
#include <fstream>
#include <sstream>

#include "graphics.h"
#include "sprite.h"
#include "rectangle.h"

Map::Map(Graphics& graphics, const std::string& map, const std::string& tile_sheet) :
    map_sprite_(std::make_shared<Sprite>(graphics, map)) { 
 
    std::ifstream map_file("content/" + map + ".txt");
    std::string map_line;
    std::getline(map_file, map_line);
    tile_size_ = stoi(map_line);

    for (map_line; std::getline(map_file, map_line);) {
        std::vector<int> row;
        for (size_t i = 0; i < map_line.size(); ++i) {
            if ((char*)map_line[i] != " ") {
                row.push_back(map_line[i] - '0');
            }
        }
        map_array_.push_back(row);
    }

    std::ifstream tile_sheet_file("content/" + tile_sheet + ".txt");
    std::string tile_sheet_line;
    std::getline(tile_sheet_file, tile_sheet_line);

    for (tile_sheet_line; std::getline(tile_sheet_file, tile_sheet_line);) {
        int tile_index = tile_sheet_line[0] - '0';
        std::vector<int> heights;
        bool empty = true;

        std::string text;
        for (size_t i = 3; i < tile_sheet_line.size(); ++i) {
            text.push_back(tile_sheet_line[i]);
        }

        std::stringstream text_stream(text);
        std::string item;
        while (std::getline(text_stream, item, ' ')) {
            heights.push_back(stoi(item));
            if (stoi(item) != 0) {
                empty = false;
            }
        }
        
        tile_sheet_heights_.emplace(std::make_pair(tile_index, heights));
        if (empty) {
            empty_tile_index_ = tile_index;
        }
    }
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
    std::vector<Map::CollisionTile> tiles;

    const int first_row = convertFloatToTile(rectangle.top());
    const int last_row = convertFloatToTile(rectangle.bottom());
    const int first_col = convertFloatToTile(rectangle.left());
    const int last_col = convertFloatToTile(rectangle.right());

    for (int row = first_row; row <= last_row; ++row) {
        for (int col = first_col; col <= last_col; ++col) {
            if (row > -1 && row < map_array_.size() && col > -1 && col < map_array_[row].size()) {
                tiles.push_back(CollisionTile(tile_sheet_heights_.at(map_array_[row][col]), row, col, map_array_[row][col]));

                if (row > 0) {
                    if (map_array_[row - 1][col] != empty_tile_index_) {
                        tiles.push_back(CollisionTile(tile_sheet_heights_.at(map_array_[row - 1][col]), row - 1, col, map_array_[row - 1][col]));
                    }
                }
                if (row < map_array_.size() - 1) {
                    if (map_array_[row + 1][col] != empty_tile_index_) {
                        tiles.push_back(CollisionTile(tile_sheet_heights_.at(map_array_[row + 1][col]), row + 1, col, map_array_[row + 1][col]));
                    }
                }
            }
        }
    }

    return tiles;
}

Map::CollisionTile Map::getCollidingTile(float x, float y) const {
    std::vector<Map::CollisionTile> tiles;

    int col = convertFloatToTile(x);
    int row = convertFloatToTile(y);

    // TODO: this can cause out-of-bounds exeption
    /*if (tiles_[row-1][col].tile_type != EMPTY_TILE) {
        row -= 1;
    }*/
    const int tile_type = map_array_[row][col];
    const std::vector<int> heights = tile_sheet_heights_.at(tile_type);

    return CollisionTile(heights, row, col, tile_type);
}

void Map::draw(Graphics& graphics) const {
    map_sprite_->draw(graphics, 0, 0);
}
