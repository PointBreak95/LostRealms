#pragma once

#include "entity.h"

using namespace std;

class Item : public Entity {
public:
    Item(const string& name, int x, int y);
    virtual string getType() const = 0;
};

class Consumable : public Item {};
class Crystal : public Consumable {};