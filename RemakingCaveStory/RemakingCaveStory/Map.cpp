#include "Map.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"
#include "Rectangle.h"

using boost::shared_ptr;
using std::vector;

// STATIC
Map* Map::createTestMap(Graphics& graphics) {
	Map* map = new Map();

	const int num_rows = 15; // 15 * 32 = 480; SCREEN HEIGHT
	const int num_cols = 20; // 20 * 32 = 640; SCREEN WIDTH

	// Ensure foreground_sprites_ is num_rows by num_cols in size.
	map->tiles_ = vector<vector<Tile>>(num_rows, vector<Tile>(num_cols, Tile()));

	shared_ptr<Sprite> sprite(new Sprite(graphics, "PrtCave", Game::kTileSize, 0, Game::kTileSize, Game::kTileSize));
	Tile tile(WALL_TILE, sprite);
	const int row = 11;
	for (int col = 0; col < num_cols; col++) {
		map->tiles_[row][col] = tile;
	}
	map->tiles_[10][5] = tile;
	map->tiles_[9][4] = tile;
	map->tiles_[8][3] = tile;
	map->tiles_[7][2] = tile;
	map->tiles_[10][3] = tile;

	return map;
}

vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
	const int first_row = rectangle.top() / Game::kTileSize;
	const int last_row = rectangle.bottom() / Game::kTileSize;
	const int first_col = rectangle.left() / Game::kTileSize;
	const int last_col = rectangle.right() / Game::kTileSize;
	
	vector<CollisionTile> collision_tiles;
	for (int row = first_row; row <= last_row; row++) {
		for (int col = first_col; col <= last_col; col++) {
			collision_tiles.push_back(CollisionTile(row, col, tiles_[row][col].tile_type));
		}
	}
	return collision_tiles;
}

void Map::update(int elapsed_time_ms) {
	for (size_t row = 0; row < tiles_.size(); row++) {
		for (size_t col = 0; col < tiles_[row].size(); col++) {
			if (tiles_[row][col].sprite) {
				tiles_[row][col].sprite->update(elapsed_time_ms);
			}
		}
	}
}

void Map::draw(Graphics& graphics) const {
	for (size_t row = 0; row < tiles_.size(); row++) {
		for (size_t col = 0; col < tiles_[row].size(); col++) {
			if (tiles_[row][col].sprite) {
				tiles_[row][col].sprite->draw(graphics, (col * Game::kTileSize), (row * Game::kTileSize));
			}
		}
	}
}