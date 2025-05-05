#include <curses.h>
#include <algorithm>
#include "map.h"
#include <thread>
#include <chrono>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 82;
const int SCREEN_HEIGHT = 30;

void drawMainScreen(WINDOW* win, const Map& map, Position player) {
    werase(win);
    box(win, 0, 0);

    for (int y = 0; y < SCREEN_HEIGHT - 2; ++y) {
        for (int x = 0; x < SCREEN_WIDTH - 2; ++x) {
            int mapX = player.x - SCREEN_WIDTH / 2 + x + 1;
            int mapY = player.y - SCREEN_HEIGHT / 2 + y + 1;

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) continue;

            if (mapX == player.x && mapY == player.y) {
                wattron(win, COLOR_PAIR(4));
                mvwaddch(win, y + 1, x + 1, '0');
                wattroff(win, COLOR_PAIR(4));
            }
            else if (map.grid[mapY][mapX].explored) {
                TileType type = map.grid[mapY][mapX].type;
                char c = map.getTileChar(type);

                int color = 1;
                switch (type) {
                case TileType::Grass:color = 7; break;
                case TileType::TallGrass:color = 7; break;
                case TileType::Water:color = 5; break;
                case TileType::Mountain:color = 3; break;
                case TileType::Trees:color = 1; break;
                case TileType::Unknown:color = 6; break;
                case TileType::Border:color = 3; break;
                }

                wattron(win, COLOR_PAIR(color));
                mvwaddch(win, y + 1, x + 1, c);
                wattroff(win, COLOR_PAIR(color));
            }
            else {
                mvwaddch(win, y + 1, x + 1, ' ');
            }
        }
    }

    wrefresh(win);
}

void revealAround(Map& map, Position player, int radius = 5) {
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -10; dx <= 10; ++dx) {
            int x = player.x + dx;
            int y = player.y + dy;
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                map.reveal({ x, y });
            }
        }
    }
}

void drawStartScreen() {
    clear();
    keypad(stdscr, TRUE);
    mvprintw(SCREEN_HEIGHT / 2 - 1, (SCREEN_WIDTH - 30) / 2, "Welcome to the Lost Realm!");
    mvprintw(SCREEN_HEIGHT / 2, (SCREEN_WIDTH - 42) / 2, "Explore the world, and collect crystals.");
    mvprintw(SCREEN_HEIGHT / 2 + 1, (SCREEN_WIDTH - 53) / 2, "Collect all the crystals to win, be aware of monsters.");
    mvprintw(SCREEN_HEIGHT / 2 + 2, (SCREEN_WIDTH - 30) / 2, "Press any arrow key to begin...");
    refresh();

    int ch;
    while (true) {
        ch = getch();
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT ||
            ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') {
            break;
        }
    }
    flushinp();
}

void drawInfoWindow(WINDOW* win, const Position& player) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Player Info");
    mvwprintw(win, 2, 1, "X: %d", player.x);
    mvwprintw(win, 3, 1, "Y: %d", player.y);
    mvwprintw(win, 4, 1, "Equipped Item: ");
    wrefresh(win);
}

void drawInstructWindow(WINDOW* win, const std::string& message) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, message.c_str());
    wrefresh(win);
}

int main() {
    initscr();
    start_color();
    use_default_colors();

    init_pair(1, COLOR_YELLOW, -1); // Trees
    init_pair(2, COLOR_BLUE, -1); // Crystals
    init_pair(3, COLOR_WHITE, -1); // Mountains & Border
    init_pair(4, COLOR_RED, -1); // Player
    init_pair(5, COLOR_CYAN, -1); // Water
    init_pair(6, COLOR_MAGENTA, -1); // Question Marks
    init_pair(7, COLOR_GREEN, -1); // Grass & TallGrass

    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); 

    drawStartScreen();

    // Create windows
    WINDOW* InstructWin = newwin(3, 40, 0, (SCREEN_WIDTH - 40) / 2);
    WINDOW* mainWin = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
    WINDOW* infoWin = newwin(10, 30, 0, SCREEN_WIDTH + 1);
    keypad(mainWin, TRUE);

    // Map Generation
    Map gameMap;
    gameMap.generateWorld();

    Position player;
    do {
        player.x = rand() % MAP_WIDTH;
        player.y = rand() % MAP_HEIGHT;
    } while (gameMap.grid[player.y][player.x].type != TileType::Grass);

    revealAround(gameMap, player);

    // Place some unknowns
    for (int i = 0; i < 15; ++i) {
        int x, y;
        do {
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;
        } while (gameMap.grid[y][x].type != TileType::Grass || gameMap.grid[y][x].explored);
        gameMap.grid[y][x].type = TileType::Unknown;
    }
    // Place some crystals, monsters, and consumables


    // Main game loop
    while (true) {
        int dx = 0, dy = 0;

        auto start = chrono::steady_clock::now();
        auto end = start + chrono::milliseconds(50);

        while (chrono::steady_clock::now() < end) {
            int ch = getch();
            switch (ch) {
            case 'q': endwin(); return 0;
            case 'e': {
                bool found = false;
                for (int dy = -1; dy <= 1 && !found; ++dy) {
                    for (int dx = -1; dx <= 1 && !found; ++dx) {
                        int tx = player.x + dx;
                        int ty = player.y + dy;
                        if (tx >= 0 && tx < MAP_WIDTH && ty >= 0 && ty < MAP_HEIGHT) {
                            TileType type = gameMap.grid[ty][tx].type;
                            if (type == TileType::Unknown) {
                                gameMap.grid[ty][tx].type = TileType::Grass; 
                                drawInstructWindow(InstructWin, "You discovered something!");
                                found = true;
                            }
                        }
                    }
                }
                if (!found) {
                    drawInstructWindow(InstructWin, "There's nothing nearby to interact with.");
                }
                break;
            }
            case KEY_UP: case 'w': dy -= 2; break;
            case KEY_DOWN: case 's': dy += 2; break;
            case KEY_LEFT: case 'a': dx -= 2; break;
            case KEY_RIGHT: case 'd': dx += 2; break;
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }   

        int newX = player.x + dx;
        int newY = player.y + dy;
        if (newX >= 0 && newX < MAP_WIDTH)  player.x = newX;
        if (newY >= 0 && newY < MAP_HEIGHT) player.y = newY;

        revealAround(gameMap, player);
        drawMainScreen(mainWin, gameMap, player);
        drawInfoWindow(infoWin, player);
        
        string tileMessage;
        switch (gameMap.grid[player.y][player.x].type) {
            case TileType::Grass:tileMessage = "You're on Grass."; break;
            case TileType::TallGrass:tileMessage = "You're in Tall Grass."; break;
            case TileType::Water:tileMessage = "You're in a River."; break;
            case TileType::Mountain:tileMessage = "You're in the Mountains."; break;
            case TileType::Trees:tileMessage = "You're in a Forest."; break;
            case TileType::Unknown:tileMessage = "There's something mysterious here."; break;
            case TileType::Border:tileMessage = "You've reached the world border."; break;
            default:tileMessage = "You're somewhere undefined."; break;
        }
        drawInstructWindow(InstructWin, tileMessage);

    }
}
