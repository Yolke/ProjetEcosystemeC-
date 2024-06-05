#include "sheep.h"
#include "grass.h"
#include "map.h"
#include "mineral.h"
#include <iostream>
#include <cstdlib>


Sheep::Sheep(int x, int y, char sex, int initialAge)
    : Entity(x, y, sex, 50, 10, 5, initialAge) {}


bool Sheep::eat(Map& map) {
    int x = getX();
    int y = getY();
    auto naturalElement = map.getNaturalElementAt(x, y);
    Grass* grass = dynamic_cast<Grass*>(naturalElement);
    if (grass != nullptr) {
        std::cout << "Sheep eats grass at position (" << x << ", " << y << ")." << std::endl;
        map.removeNatural(x, y); // Supprime l'herbe de la carte
        hunger = 0; // Réinitialise la faim du mouton
        return true;
    }
    return false;
}

bool Sheep::reproduce(Map& map) {
    int x = getX();
    int y = getY();
    auto matePosition = isAdjacentToMate(map, 'S');
    if (matePosition.first != -1 && matePosition.second != -1) {
        int newX = matePosition.first;
        int newY = matePosition.second;
        char newSex = (rand() % 2 == 0) ? 'M' : 'F'; // Sexe aléatoire pour le nouvel enfant
        map.addEntity(std::make_shared<Sheep>(newX, newY, newSex, 0), newX, newY);
        std::cout << "Sheep at (" << x << ", " << y << ") and Sheep at (" << newX << ", " << newY << ") reproduced. New sheep born at (" << newX << ", " << newY << ").\n";
        return true;
    }
    return false;
}

void Sheep::update(Map& map) {
    age++; // Toujours incrémenter l'âge au début de la mise à jour
    if (eat(map)) { // Essayer de manger de l'herbe
        move(map);
        ageAndHunger();
        return; // Termine le tour si mangé
    } 
    hunger++; // Augmenter la faim si rien n'a été mangé

    if (reproduce(map)) { // Essayer de se reproduire
        move(map);
        ageAndHunger();
        return; // Termine le tour si reproduction réussie
    }

    move(map); // Déplace le mouton si ni mangé ni reproduit
    ageAndHunger(); // Mettre à jour l'âge et la faim après le mouvement

    if (!isAlive) {
        // Créer un minéral à l'emplacement du mouton mort
        map.addNatural(std::make_shared<Mineral>(getX(), getY()), getX(), getY());

    }
}

