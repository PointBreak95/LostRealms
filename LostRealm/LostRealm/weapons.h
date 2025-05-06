#pragma once

#include "items.h"
#include <string>

using namespace std;

class Weapon : public Item {
public:
    Weapon(const string& name, int x, int y, int id, float attackMultiplier);

    int getID() const;
    float getAttackMultiplier() const;

    string getType() const override;

protected:
    int id;
    float attackMultiplier;

};

class Sword : public Weapon {
public:
    Sword(int x, int y) : Weapon("Sword", x, y, 0, 1.2f) {}
};

class Dagger : public Weapon {
public:
    Dagger(int x, int y) : Weapon("Dagger", x, y, 1, 1.1f) {}
};

class Staff : public Weapon {
public:
    Staff(int x, int y) : Weapon("Staff", x, y, 2, 0.6f) {}
};

class Bow : public Weapon {
public:
    Bow(int x, int y) : Weapon("Bow", x, y, 3, 1.4f) {}
};

class Spear : public Weapon {
public:
    Spear(int x, int y) : Weapon("Spear", x, y, 4, 1.5f) {}
};