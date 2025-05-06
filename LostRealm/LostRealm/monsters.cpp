#include "monsters.h"
#include <string>

using namespace std;

Monster::Monster(int x, int y, int type):Entity("", x, y) {
    
    string monsterName;
    int minLevel = 1, maxLevel = 1;

    switch (type) {
    case 0:
        monsterName = "Goblin";
        id = 0; 
        minLevel = 1; 
        maxLevel = 3;
        hp = 20; 
        attack = 5; 
        break;
    case 1:
        monsterName = "Skeleton";
        id = 1; 
        minLevel = 2;
        maxLevel = 5;
        hp = 10;
        attack = 10;
        break;
    case 2:
        monsterName = "Orc";
        id = 2;
        minLevel = 4;
        maxLevel = 6;
        hp = 40;
        attack = 5;
        break;
    case 3:
        monsterName = "Dark Mage";
        id = 3;
        minLevel = 5;
        maxLevel = 7;
        hp = 20;
        attack = 20;
        break;
    case 4:
        monsterName = "Demon";
        id = 4;
        minLevel = 5;
        maxLevel = 9;
        hp = 30;
        attack = 20;
        break;
    case 5:
        monsterName = "Dragon";
        id = 5;
        minLevel = 8;
        maxLevel = 10;
        hp = 50;
        attack = 30;
        break;
    default:
        monsterName = "Unknown";
        id = -1;
        minLevel = 10;
        maxLevel = 10;
        hp = 200;
        attack = 500;
        break;
    }

    currentHP = hp;
    maxHP = hp;

    level = minLevel + rand() % (maxLevel - minLevel + 1);

    name = monsterName + " (Lv. " + to_string(level) + ")";
}

int Monster::getHP() const { return hp; }
int Monster::getAttack() const { return attack; }