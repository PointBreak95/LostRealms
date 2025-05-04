#include "map.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Map::Map() {
    grid.resize(MAP_HEIGHT, vector<Tile>(MAP_WIDTH));
}

void Map::generateWorld() {
    srand(time(0));

    for (int y = 0; y < MAP_HEIGHT; ++y)
        for (int x = 0; x < MAP_WIDTH; ++x)
            grid[y][x].type = TileType::Grass;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1) {
                grid[y][x].type = TileType::Border;
            }
        }
    }

    auto blobbyGrow = [&](int cx, int cy, int targetSize, TileType type) {
        vector<Position> frontier = { {cx, cy} };
        int count = 0;

        while (!frontier.empty() && count < targetSize) {
            Position p = frontier.back();
            frontier.pop_back();

            if (p.x < 1 || p.x >= MAP_WIDTH - 1 || p.y < 1 || p.y >= MAP_HEIGHT - 1)
                continue;
            if (grid[p.y][p.x].type != TileType::Grass)
                continue;

            grid[p.y][p.x].type = type;
            count++;

            for (int i = 0; i < 4; ++i) {
                int dx = rand() % 3 - 1;
                int dy = rand() % 3 - 1;
                frontier.push_back({ p.x + dx, p.y + dy });
            }
        }
        };

    for (int i = 0; i < 3 + rand() % 2; ++i) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % (MAP_HEIGHT / 2);
        int size = 100 + rand() % 80;
        blobbyGrow(x, y, size, TileType::Trees);
    }
	
    for (int i = 0; i < 3 + rand() % 2; ++i) {
		int x = rand() % MAP_WIDTH;
		int y = rand() % (MAP_HEIGHT / 2);
		int size = 100 + rand() % 80;
		blobbyGrow(x, y, size, TileType::Mountain);
	}

    for (int i = 0; i < 3 + rand() % 2; ++i) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % (MAP_HEIGHT / 2);
        int size = 100 + rand() % 80;
        blobbyGrow(x, y, size, TileType::Trees);
    }

    for (int y = 1; y < MAP_HEIGHT - 1; ++y) {
		for (int x = 1; x < MAP_WIDTH - 1; ++x) {
			if (grid[y][x].type == TileType::Grass) {
				for (int dy = -1; dy <= 1; ++dy)
					for (int dx = -1; dx <= 1; ++dx)
						if (grid[y + dy][x + dx].type == TileType::Trees)
					        grid[y][x].type = TileType::TallGrass;
			}
		}
	}

    auto riverWalk = [&](int x, int y, int dir, int length) {
        for (int i = 0; i < length && x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT; ++i) {
            int width = 2 + rand() % 3;
            for (int w = -width / 2; w <= width / 2; ++w) {
                int nx = x + w, ny = y;
                if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT)
                    grid[ny][nx].type = TileType::Water;
            }
            x += dir;
            y += rand() % 3 - 1;
        }
        };

    int startY1 = rand() % MAP_HEIGHT;
    int startY2 = rand() % MAP_HEIGHT;
    
    int riverCount = 2 + rand() % 4; // 2 to 5 rivers
    for (int i = 0; i < riverCount; ++i) {
        int startY = rand() % MAP_HEIGHT;
        int dir = (i % 2 == 0) ? 1 : -1;
        int startX = (dir == 1) ? 0 : MAP_WIDTH - 1;
        riverWalk(startX, startY, dir, MAP_WIDTH / 2 + rand() % (MAP_WIDTH / 4));
    }
   
}



void Map::reveal(Position pos) {
    if (pos.x >= 0 && pos.x < MAP_WIDTH && pos.y >= 0 && pos.y < MAP_HEIGHT)
        grid[pos.y][pos.x].explored = true;
}

char Map::getTileChar(TileType t) const {
    switch (t) {
    case TileType::Grass: return '.';
    case TileType::Water: return '~';
    case TileType::Mountain: return '^';
    case TileType::TallGrass: return ',';
    case TileType::Trees: return '!';
    case TileType::Border: return 'o';
    default: return ' ';
    }
}