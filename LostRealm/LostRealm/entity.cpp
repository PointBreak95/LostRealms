#include "entity.h"

using namespace std;

Entity::Entity(const string& name, int x, int y)
    : name(name), x(x), y(y) {
}

int Entity::getX() const { return x; }
int Entity::getY() const { return y; }

void Entity::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

string Entity::getName() const {
    return name;
}
