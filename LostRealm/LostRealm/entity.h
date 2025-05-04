#pragma once

#include <string>

using namespace std;

class Entity {
protected:
    string name;
    int x, y;

public:
    Entity(const string& name, int x, int y);
    virtual ~Entity() = default;

    string getName() const;
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
};

