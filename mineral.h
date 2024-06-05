#pragma once

#include "natural.h"

class Mineral : public NaturalElement {
    int age;  // Age counter for the mineral
    static constexpr int TRANSFORMATION_AGE = 1;
public:
    Mineral(int x, int y);
    virtual void update(Map& map) override;
    virtual char getSymbol() const override;
    void incrementAge();
    int getAge() const;
    bool shouldTransformToGrass() const;
};