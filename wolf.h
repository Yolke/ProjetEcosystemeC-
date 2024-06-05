#pragma once
#include "entity.h"

class Wolf : public Entity {
public:
    Wolf(int x, int y, char sex, int initialAge = 0);
    void update(Map& map) override;
    char getSymbol() const override { return 'W'; }
    bool eatSheep(Map& map);
    bool reproduce(Map& map);

};