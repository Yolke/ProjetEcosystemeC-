#pragma once
#include "entity.h"

class Sheep : public Entity {
public:
    Sheep(int x, int y, char sex, int initialAge = 0);
    void update(Map& map) override;
    bool eat(Map& map);
    bool reproduce(Map& map);
    char getSymbol() const override { return 'S'; }
};
