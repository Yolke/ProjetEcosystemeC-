#include "mineral.h"
#include "map.h"


Mineral::Mineral(int x, int y) : NaturalElement(x, y), age(0) {}

void Mineral::update(Map& map) {
    incrementAge();
    if (getAge() >= 3) {
        map.transformMineralToGrass(x, y);
    }
}
bool Mineral::shouldTransformToGrass() const {
    return age >= 3; // Transformer en herbe si l'âge est supérieur ou égal à 3
}

char Mineral::getSymbol() const {
    return 'M';
}

void Mineral::incrementAge() {
    age++;
}

int Mineral::getAge() const {
    return age;
}