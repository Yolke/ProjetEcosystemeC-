#include "natural.h"

NaturalElement::NaturalElement(int x, int y) : x(x), y(y) {}

NaturalElement::~NaturalElement() {}

int NaturalElement::getX() const {
    return x;
}

int NaturalElement::getY() const {
    return y;
}
