#pragma once

#include "natural.h"

class Grass : public NaturalElement {
public:
    Grass(int x, int y);
    virtual void update(Map& map) override;
    virtual char getSymbol() const override;
};