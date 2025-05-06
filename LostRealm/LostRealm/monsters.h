#pragma once
#include "entity.h"
#include <string>

class Monster : public Entity {
public:
    Monster(int x, int y, int type);

    int getHP() const;
    int getAttack() const;

    int getCurrentHP() const { return currentHP; }
    void setCurrentHP(int hp) { currentHP = hp; }

private:
    int id;
    int level;

    int hp;
	int currentHP;
	int maxHP;

    int attack;

};
