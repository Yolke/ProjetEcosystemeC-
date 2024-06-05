#include "map.h"
#include "grass.h"


Grass::Grass(int x, int y) : NaturalElement(x, y) {}

void Grass::update(Map& map) {
    map.transformMineralToGrass(x, y);
}

char Grass::getSymbol() const {
    return 'G';
}