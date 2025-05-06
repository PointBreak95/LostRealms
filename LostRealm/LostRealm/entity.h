#pragma once
#include <string>

using namespace std;

class Entity {
public:
    Entity(const string& name, int x, int y);

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

    string getName() const;

protected:
    string name;
    int x, y;
};
