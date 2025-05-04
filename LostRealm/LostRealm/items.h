#pragma once

#include "entity.h"

class Item : public Entity {
public:
    Item(const std::string& name, int x, int y);
    virtual std::string getType() const = 0;
};

class Weapon : public Item {};
class Sword : public Weapon {};
class Shield : public Weapon {};
class Dagger : public Weapon {};
class Staff : public Weapon {};
class Bow : public Weapon {};
class Spear : public Weapon {};

class Consumable : public Item {};
class ManaPotion : public Consumable {};
class HealthPotion : public Consumable {};