#include "map.h"
#include "natural.h" // Assurez-vous que ce fichier est inclus
#include "entity.h"  // Pour Entity si nécessaire
#include "grass.h"
#include "wolf.h"
#include "sheep.h"
#include "mineral.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

Map::Map(int width, int height) : width(width), height(height) {
    // Redimensionner les vecteurs pour stocker les entités et les éléments naturels
    entities.resize(height, std::vector<std::shared_ptr<Entity>>(width, nullptr));
    naturals.resize(height, std::vector<std::shared_ptr<NaturalElement>>(width, nullptr));

    // Initialiser chaque case avec de l'herbe et vide pour les entités
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            naturals[y][x] = std::make_shared<Grass>(x, y);  // Ajoute de l'herbe sur chaque case
            // Les cases entités sont initialisées à nullptr par défaut
        }
    }
}


void Map::addEntity(std::shared_ptr<Entity> entity, int x, int y) {
    if (isValidPosition(x, y)) {
        entities[y][x] = entity;
    }
}

void Map::addNatural(std::shared_ptr<NaturalElement> natural, int x, int y) {
    if (isValidPosition(x, y)) {
        naturals[y][x] = natural;
        std::cout << "Natural element of symbol '" << natural->getSymbol() << "' added at position (" << x << ", " << y << ").\n";
    }
    
}

void Map::transformMineralToGrass(int x, int y) {
    auto& cell = naturals[y][x];
    if (cell) {  // Vérifie si le pointeur est valide
        Mineral* mineral = dynamic_cast<Mineral*>(cell.get());
        if (mineral && mineral->getAge() >= 3) {
            cell.reset();  // Réinitialise le pointeur
            addNatural(std::make_shared<Grass>(x, y), x, y);  // Ajoutez de l'herbe
        }
    }
}

void Map::printNaturalMap() const {
    std::cout << "+";
    for (int x = 0; x < width; x++) {
        std::cout << "---+";
    }
    std::cout << "\n";
    for (int y = 0; y < height; y++) {
        std::cout << "|";
        for (int x = 0; x < width; x++) {
            char symbol = ' ';
            if (naturals[y][x]) {
                symbol = naturals[y][x]->getSymbol();
            }
            std::cout << " " << symbol << " |";
        }
        std::cout << "\n";
        std::cout << "+";
        for (int x = 0; x < width; x++) {
            std::cout << "---+";
        }
        std::cout << "\n";
    }
}


void Map::update() {
    // Copie temporaire des entités pour éviter de modifier les entités en cours de parcours
    auto tempEntities = entities;

    // Parcours des entités et des éléments naturels dans chaque case
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Mise à jour de l'entité présente dans la case
            if (tempEntities[y][x]) {
                auto entity = tempEntities[y][x];
                entity->update(*this);
                if (entity->shouldBeRemoved()) {
                    removeEntity(x, y);
                } else {
                    // Mettre à jour la position de l'entité dans la carte
                    int newX = entity->getX();
                    int newY = entity->getY();
                    if (newX != x || newY != y) {
                        // Si la position a changé, déplacez l'entité dans la carte
                        removeEntity(x, y);
                        addEntity(entity, newX, newY);
                    }
                }
            }

            // Vérification et transformation des minéraux en herbe
            auto& cell = naturals[y][x];
            if (cell) {  
                Mineral* mineral = dynamic_cast<Mineral*>(cell.get());
                if (mineral) {
                    mineral->update(*this); // Appelle la méthode update de Mineral
                }
            }
        }
    }
}

void Map::printMap() const {
    std::cout << "+";
    for (int x = 0; x < width; x++) {
        std::cout << "---+";
    }
    std::cout << "\n";
    for (int y = 0; y < height; y++) {
        std::cout << "|";
        for (int x = 0; x < width; x++) {
            char symbol = ' ';
            if (entities[y][x]) {
                symbol = entities[y][x]->getSymbol();
            }
            std::cout << " " << symbol << " |";
        }
        std::cout << "\n";
        std::cout << "+";
        for (int x = 0; x < width; x++) {
            std::cout << "---+";
        }
        std::cout << "\n";
    }

}
bool Map::isAdjacentToMate(int x, int y, char symbol) const {
    // Vérifie si une entité compatible pour la reproduction du type 'symbol' est adjacente à la position (x, y)
    std::vector<std::pair<int, int>> adjacentPositions = {
        {x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1},
        {x - 1, y},                 {x + 1, y},
        {x - 1, y + 1}, {x, y + 1}, {x + 1, y + 1}
    };

    for (const auto& [adjX, adjY] : adjacentPositions) {
        if (isValidPosition(adjX, adjY)) {
            std::shared_ptr<Entity> entity = entities[adjY][adjX];
            if (entity && entity->getSymbol() == symbol && entity->canMate()) {
                return true;
            }
        }
    }

    return false;
}


void Map::mateEntities(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2) {
    // Méthode pour faire se reproduire deux entités, en supposant que entity1 et entity2 sont déjà validés pour la reproduction
    int x1 = entity1->getX();
    int y1 = entity1->getY();
    int x2 = entity2->getX();
    int y2 = entity2->getY();

    std::vector<std::pair<int, int>> adjacentPositions = {
        {x1 - 1, y1}, {x1 + 1, y1}, {x1, y1 - 1}, {x1, y1 + 1},
        {x2 - 1, y2}, {x2 + 1, y2}, {x2, y2 - 1}, {x2, y2 + 1}
    };

    for (const auto& [adjX, adjY] : adjacentPositions) {
        if (isValidPosition(adjX, adjY) && isCellEmpty(adjX, adjY)) {
            char newSex = rand() % 2 == 0 ? 'M' : 'F'; // Sexe aléatoire pour le nouvel enfant
            if (entity1->getSymbol() == 'W' || entity2->getSymbol() == 'W') {
                addEntity(std::make_shared<Wolf>(adjX, adjY, newSex), adjX, adjY);
            } else {
                addEntity(std::make_shared<Sheep>(adjX, adjY, newSex), adjX, adjY);
            }
            break; // Arrêter après avoir trouvé une position libre
        }
    }
}


NaturalElement* Map::getNaturalElementAt(int x, int y) const {
    if (isValidPosition(x, y)) {
        return naturals[y][x].get();
    }
    return nullptr;
}

void Map::removeNatural(int x, int y) {
    if (isValidPosition(x, y)) {
        naturals[y][x].reset();
    }
}
bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Map::isCellEmpty(int x, int y) const {
    return !entities[y][x]; // La case est vide si elle ne contient pas d'entité
}

std::shared_ptr<Entity> Map::getEntityAt(int x, int y) const {
    if (!isValidPosition(x, y)) return nullptr;
    return entities[y][x];
}



void Map::removeEntity(int x, int y) {
    if (isValidPosition(x, y)) {
        entities[y][x].reset();  // Supprime l'entité en réinitialisant le pointeur
    }
}
bool Map::hasLivingEntities() const {
    for (const auto& row : entities) {
        for (const auto& entity : row) {
            if (entity != nullptr) {
                return true;
            }
        }
    }
    return false;
}
void Map::printEntityCounts() const {
    int sheepCount = 0;
    int wolfCount = 0;
    for (const auto& row : entities) {
        for (const auto& entity : row) {
            if (entity) {
                if (entity->getSymbol() == 'S') {
                    sheepCount++;
                } else if (entity->getSymbol() == 'W') {
                    wolfCount++;
                }
            }
        }
    }
    std::cout << "Nombre de moutons restants : " << sheepCount << std::endl;
    std::cout << "Nombre de loups restants : " << wolfCount << std::endl;
}
std::pair<int, int> Map::getEntityCounts() const {
    int sheepCount = 0;
    int wolfCount = 0;
    for (const auto& row : entities) {
        for (const auto& entity : row) {
            if (entity) {
                if (entity->getSymbol() == 'S') {
                    sheepCount++;
                } else if (entity->getSymbol() == 'W') {
                    wolfCount++;
                }
            }
        }
    }
    return {wolfCount, sheepCount};
}
