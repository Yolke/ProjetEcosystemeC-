#include "entity.h"
#include <cstdlib>  // Pour la fonction rand()
#include <iostream> // Pour std::cout
#include "map.h"
#include "mineral.h"

Entity::Entity(int x, int y, char sex, int maxAge, int maxHunger, int maturityAge, int age)
    : x(x), y(y), sex(sex), age(age), maxAge(maxAge), maxHunger(maxHunger), maturityAge(maturityAge), isAlive(true) {}


Entity::~Entity() {}

bool Entity::shouldBeRemoved() const {
    return !isAlive || age > maxAge || hunger > maxHunger;
}

int Entity::getX() const { return x; }
int Entity::getY() const { return y; }
char Entity::getSex() const { return sex; }
void Entity::setSex(char newSex) { sex = newSex; }
bool Entity::canMate() const { return age >= maturityAge && hunger < maxHunger; }

std::pair<int, int> Entity::isAdjacentToMate(Map& map, char symbol) const {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx != 0 || dy != 0) {  // Ne pas vérifier la position actuelle
                int newX = x + dx, newY = y + dy;
                if (map.isValidPosition(newX, newY)) {
                    auto entity = map.getEntityAt(newX, newY);
                    if (entity && entity->getSymbol() == symbol && entity->getSex() != sex && entity->canMate()) {
                        return std::make_pair(newX, newY); // Retourne les coordonnées de l'entité compatible
                    }
                }
            }
        }
    }
    return std::make_pair(-1, -1); // Si aucune entité compatible n'est trouvée, retourne une paire d'entiers invalides
}


void Entity::move(Map& map) {
    int dx = rand() % 3 - 1; // génère -1, 0, ou 1
    int dy = rand() % 3 - 1;
    int newX = x + dx;
    int newY = y + dy;

    if (map.isValidPosition(newX, newY) && map.isCellEmpty(newX, newY)) {
        x = newX;
        y = newY;
    }
}


void Entity::ageAndHunger() {
    if (age > maxAge || hunger > maxHunger) {
        isAlive = false;
        std::cout << "Entity " << getSymbol() << " died at position (" << x << ", " << y << ") ";
        if (age > maxAge) {
            std::cout << "due to old age.\n";
        } else {
            std::cout << "due to starvation.\n";
        }
    }
}





