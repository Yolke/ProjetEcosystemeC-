#pragma once

class Map;

class NaturalElement {
protected:
    int x, y;

public:
    NaturalElement(int x, int y);
    virtual ~NaturalElement();
    virtual void update(Map& map) = 0;
    virtual char getSymbol() const = 0;

    int getX() const;
    int getY() const;
};

