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

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
                continue;

            if (mapX == player.x && mapY == player.y) {
                wattron(win, COLOR_PAIR(5));
                mvwaddch(win, y + 1, x + 1, '@');
                wattroff(win, COLOR_PAIR(5));
            }
            else if (map.grid[mapY][mapX].explored) {
                TileType type = map.grid[mapY][mapX].type;
                char c = map.getTileChar(type);

                int color = 1;
                switch (type) {
                case TileType::Grass:color = 8; break;
                case TileType::TallGrass:color = 8; break;
                case TileType::Water:color = 6; break;
                case TileType::Mountain:color = 3; break;
                case TileType::Trees:color = 1; break;
                case TileType::Unknown:color = 7; break;
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
        // Accept both arrow keys and common fallback codes
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT || ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') {
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

enum class ActionType { None, Move, Quit, Interact };

struct InputAction {
    ActionType type;
    int dx = 0;
    int dy = 0;
};

InputAction handleInput() {
    InputAction action;

    int ch = getch();
    switch (ch) {
    case 'q': action.type = ActionType::Quit; break;
    case 'e': action.type = ActionType::Interact; break;
    case KEY_UP: case 'w': action = { ActionType::Move, 0, -2 }; break;
    case KEY_DOWN: case 's': action = { ActionType::Move, 0, 2 }; break;
    case KEY_LEFT: case 'a': action = { ActionType::Move, -2, 0 }; break;
    case KEY_RIGHT: case 'd': action = { ActionType::Move, 2, 0 }; break;
    }

    return action;
}

int main() {
    initscr();

    start_color();
    use_default_colors();
    init_pair(1, COLOR_YELLOW, -1);     
    init_pair(2, COLOR_BLUE, -1);      
    init_pair(3, COLOR_WHITE, -1);     
    init_pair(4, COLOR_YELLOW, -1);    
    init_pair(5, COLOR_RED, -1);       
    init_pair(6, COLOR_CYAN, -1);      
    init_pair(7, COLOR_MAGENTA, -1);   
    init_pair(8, COLOR_GREEN, -1);

    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    drawStartScreen();

    int InstructWinWidth = 40;
    int InstructWinHeight = 3;
    int InstructWinStartY = 0;
    int InstructWinStartX = (SCREEN_WIDTH - InstructWinWidth) / 2;

    WINDOW* InstructWin = newwin(3, 40, 0, (SCREEN_WIDTH - InstructWinWidth) / 2);

    WINDOW* mainWin = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

    WINDOW* infoWin = newwin(10, 30, 0, SCREEN_WIDTH + 1);

    keypad(mainWin, TRUE);

    Map gameMap;
	gameMap.generateWorld();

    Position player; // Randomly place the player on a grass tile

    do {
        player.x = rand() % MAP_WIDTH;
        player.y = rand() % MAP_HEIGHT;
    } while (gameMap.grid[player.y][player.x].type != TileType::Grass);

	revealAround(gameMap, player); // Reveal the area around the player

	// Place some unknown tiles in unexplored area
    for (int i = 0; i < 10; ++i) {
        int x, y;
        do {
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;
        } while (gameMap.grid[y][x].type != TileType::Grass || gameMap.grid[y][x].explored);
        gameMap.grid[y][x].type = TileType::Unknown;
    }

    while (true) {
        InputAction action = handleInput();

        switch (action.type) {
        case ActionType::Move: {
            int newX = player.x + action.dx;
            int newY = player.y + action.dy;
            if (newX >= 0 && newX < MAP_WIDTH) player.x = newX;
            if (newY >= 0 && newY < MAP_HEIGHT) player.y = newY;
            break;
        }
        case ActionType::Interact:
            // Placeholder: Add item/monster interaction here
            drawInstructWindow(InstructWin, "There's nothing to interact with.");
            break;

        case ActionType::Quit:
            endwin();
            return 0;

        default:
            break;
        }

        revealAround(gameMap, player);
        drawMainScreen(mainWin, gameMap, player);
        drawInfoWindow(infoWin, player);
        drawInstructWindow(InstructWin, "Use arrow keys to explore.");

        this_thread::sleep_for(chrono::milliseconds(50));
    }

}
