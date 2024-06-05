#include "wolf.h"
#include "map.h"
#include <cstdlib>
#include "mineral.h"

Wolf::Wolf(int x, int y, char sex, int initialAge) : Entity(x, y, sex, 60, 10, 5, initialAge) {}


bool Wolf::eatSheep(Map& map) {
    int x = getX();
    int y = getY();

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx != 0 || dy != 0) {
                int newX = x + dx, newY = y + dy;
                if (map.isValidPosition(newX, newY)) {
                    auto entity = map.getEntityAt(newX, newY);
                    if (entity && entity->getSymbol() == 'S') {
                        std::cout << "Wolf at (" << x << ", " << y << ") ate Sheep at (" << newX << ", " << newY << ").\n";
                        map.removeEntity(newX, newY);
                        hunger = 0;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Wolf::reproduce(Map& map) {
    int x = getX();
    int y = getY();
    auto matePosition = isAdjacentToMate(map, 'W');
    if (matePosition.first != -1 && matePosition.second != -1) {
        int newX = matePosition.first;
        int newY = matePosition.second;
        char newSex = (rand() % 2 == 0) ? 'M' : 'F';
        map.addEntity(std::make_shared<Wolf>(newX, newY, newSex, 0), newX, newY);
        std::cout << "Wolf at (" << x << ", " << y << ") and Wolf at (" << newX << ", " << newY << "0) reproduced. New wolf born at (" << newX << ", " << newY << ")\n";
        return true;
    }
    return false;
}

void Wolf::update(Map& map) {
    age++;
    if (eatSheep(map)) {
        move(map);
        ageAndHunger();
        return; 
    }
    
    hunger++;

    if (reproduce(map)) {
        move(map);
        ageAndHunger();
        return; 
    }

    move(map); // Move the wolf if neither eating nor reproducing happened
    ageAndHunger();
    if (!isAlive) {
        // Créer un minéral à l'emplacement du mouton mort
        map.addNatural(std::make_shared<Mineral>(getX(), getY()), getX(), getY());


    }
}

