#include "entity.h"
#include <string>

using namespace std;

Entity::Entity(const string& name, int x, int y)
    : name(name), x(x), y(y) {
}

string Entity::getName() const { return name; }
int Entity::getX() const { return x; }
int Entity::getY() const { return y; }

void Entity::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

