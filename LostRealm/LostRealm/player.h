#pragma once
#include <string>

class Player {
public:
    int x, y;

    int maxHP;
    int currentHP;
    int attack;
    int defense;

    int getAttack() const { return attack; }

    Player(int startX, int startY);

};