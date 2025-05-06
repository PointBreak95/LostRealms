#include "weapons.h"

using namespace std;

Weapon::Weapon(const string& name, int x, int y, int id, float attackMult): Item(name, x, y) {
	
    this->id = id;
    this->attackMultiplier = attackMult;

}

int Weapon::getID() const {
    return id;
}

float Weapon::getAttackMultiplier() const {
    return attackMultiplier;
}

string Weapon::getType() const {
    return "Weapon";
}