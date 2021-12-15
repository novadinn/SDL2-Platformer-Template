#include "map.h"

#include "graphics.h"
#include "sprite.h"
#include "rectangle.h"

namespace {
    const std::map<Map::TileType, std::vector<int >> kHeightTable{
        {Map::EMPTY_TILE, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
        {Map::WALL_TILE, {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16}},
        {Map::LEFT_TRIANGLE_TILE, {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1}},
        {Map::RIGHT_TRIANGLE_TILE, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}},
        {Map::LOW_TILE, {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4}},
        {Map::MEDIUM_TILE, {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8}},
        {Map::HIGH_TILE, {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12}},
    };
}

Map* Map::createTestMap(Graphics& graphics) {
    Map* map = new Map();

    const int num_rows = 30;
    const int num_cols = 120;
    map->tiles_ = std::vector<std::vector<Tile>>(num_rows, std::vector<Tile>(num_cols, Tile()));

    std::shared_ptr<Sprite> sprite(new Sprite(graphics, "tile", 0, 0, generics::kTileSize, generics::kTileSize));
    Tile tile(WALL_TILE, sprite);

    std::shared_ptr<Sprite> left_trinagle_tile_sprite(new Sprite(graphics, "left_triangle_tile", 0,0, generics::kTileSize, generics::kTileSize));
    Tile left_triangle_tile(LEFT_TRIANGLE_TILE, left_trinagle_tile_sprite);
    std::shared_ptr<Sprite> right_triangle_sprite (new Sprite(graphics, "right_triangle_tile", 0, 0, generics::kTileSize, generics::kTileSize));
    Tile right_triangle_tile(RIGHT_TRIANGLE_TILE, right_triangle_sprite);

    std::shared_ptr<Sprite> low_tile_sprite(new Sprite(graphics, "low_tile", 0, 0, generics::kTileSize, generics::kTileSize));
    Tile low_tile(LOW_TILE, low_tile_sprite);
    std::shared_ptr<Sprite> medium_tile_sprite(new Sprite(graphics, "medium_tile", 0, 0, generics::kTileSize, generics::kTileSize));
    Tile medium_tile(MEDIUM_TILE, medium_tile_sprite);
    std::shared_ptr<Sprite> high_tile_sprite(new Sprite(graphics, "high_tile", 0, 0, generics::kTileSize, generics::kTileSize));
    Tile high_tile(HIGH_TILE, high_tile_sprite);

    for (int col = 0; col < num_cols; ++col) {
        for (int row = 0; row < num_rows; ++row) {
            map->tiles_[row][col] = Tile(EMPTY_TILE, nullptr);
        }
    }

    for (int col = 0; col < num_cols; ++col) {
        for(int row = 24; row < 30; ++row)
            map->tiles_[row][col] = tile;
        for (int row = 0; row < 2; ++row)
            map->tiles_[row][col] = tile;
    }

    for (int i = 0; i < num_rows; ++i) {
        map->tiles_[i][0] = tile;
        map->tiles_[i][num_cols-1] = tile;
    }

    for (int col = 30; col < 39; col+=2) {
        map->tiles_[23][col] = right_triangle_tile;
        map->tiles_[23][col + 1] = left_triangle_tile;
    }

    map->tiles_[23][21] = low_tile;
    map->tiles_[23][22] = low_tile;
    map->tiles_[23][23] = medium_tile;
    map->tiles_[23][24] = medium_tile;
    map->tiles_[23][25] = high_tile;
    map->tiles_[23][26] = tile;

    {
        const int row = 14;
        const int col = 14;
        map->tiles_[row][col] = tile;
        map->tiles_[row][col+1] = tile;
        map->tiles_[row][col+2] = left_triangle_tile; // left..
        map->tiles_[row+1][col + 3] = left_triangle_tile;
        map->tiles_[row+2][col + 4] = left_triangle_tile;
        map->tiles_[row+3][col + 5] = left_triangle_tile;
        map->tiles_[row+4][col + 6] = left_triangle_tile; // left
        map->tiles_[row + 1][col + 2] = tile;
        map->tiles_[row + 2][col + 3] = tile;
        map->tiles_[row + 3][col + 4] = tile;
        map->tiles_[row + 4][col + 5] = tile;
        map->tiles_[row + 5][col + 6] = tile;
    }

    {
        const int row = 18;
        const int col = 12;
        map->tiles_[row][col] = tile;
        map->tiles_[row][col + 1] = tile;
        map->tiles_[row][col - 1] = right_triangle_tile;
        map->tiles_[row + 1][col - 2] = right_triangle_tile;
        map->tiles_[row + 2][col - 3] = right_triangle_tile;
        map->tiles_[row + 3][col - 4] = right_triangle_tile;
        map->tiles_[row + 4][col - 5] = right_triangle_tile;
        map->tiles_[row + 1][col - 1] = tile;
        map->tiles_[row + 2][col - 2] = tile;
        map->tiles_[row + 3][col - 3] = tile;
        map->tiles_[row + 4][col - 4] = tile;
        map->tiles_[row + 5][col - 5] = tile;
    }

    map->tiles_[23][3] = tile;
    map->tiles_[23][4] = tile;
    map->tiles_[22][3] = tile;
    map->tiles_[22][4] = tile;

    return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
    std::vector<Map::CollisionTile> tiles;

    const int first_row = generics::convertFloatToTile(rectangle.top());
    const int last_row = generics::convertFloatToTile(rectangle.bottom());
    const int first_col = generics::convertFloatToTile(rectangle.left());
    const int last_col = generics::convertFloatToTile(rectangle.right());

    for (int row = first_row; row <= last_row; ++row) {
        for (int col = first_col; col <= last_col; ++col) {
            tiles.push_back(CollisionTile(kHeightTable.at(tiles_[row][col].tile_type), row, col, tiles_[row][col].tile_type));
            if (tiles_[row - 1][col].tile_type != EMPTY_TILE) {
                tiles.push_back(CollisionTile(kHeightTable.at(tiles_[row-1][col].tile_type), row-1, col, tiles_[row-1][col].tile_type));
            }
            if (tiles_[row + 1][col].tile_type != EMPTY_TILE) {
                tiles.push_back(CollisionTile(kHeightTable.at(tiles_[row + 1][col].tile_type), row + 1, col, tiles_[row + 1][col].tile_type));
            }
        }
    }

    return tiles;
}

Map::CollisionTile Map::getCollidingTile(float x, float y) const {
    std::vector<Map::CollisionTile> tiles;

    int col = generics::convertFloatToTile(x);
    int row = generics::convertFloatToTile(y);

    // TODO: this can cause out-of-bounds exeption
    /*if (tiles_[row-1][col].tile_type != EMPTY_TILE) {
        row -= 1;
    }*/
    const TileType tile_type = tiles_[row][col].tile_type;
    const std::vector<int> heights = kHeightTable.at(tile_type);

    return CollisionTile(heights, row, col, tile_type);
}

void Map::draw(Graphics& graphics) const {
    for (size_t row = 0; row < tiles_.size(); ++row) {
        for (size_t col = 0; col < tiles_[row].size(); ++col) {
            if (tiles_[row][col].sprite) {
                tiles_[row][col].sprite->draw(graphics, generics::convertTileToFloat(col), generics::convertTileToFloat(row));
            }
        }
    }
}
