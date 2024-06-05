#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include "mineral.h"

class Entity; // Forward declaration
class NaturalElement; // Forward declaration

class Map {
private:
    int width;
    int height;
    std::vector<std::vector<std::shared_ptr<Entity>>> entities;
    std::vector<std::vector<std::shared_ptr<NaturalElement>>> naturals;

public:
    Map(int width, int height);
    void addEntity(std::shared_ptr<Entity> entity, int x, int y);
    void addNatural(std::shared_ptr<NaturalElement> natural, int x, int y);
    void update();
    void printMap() const;
    bool isValidPosition(int x, int y) const;
    bool isCellEmpty(int x, int y) const;
    std::shared_ptr<Entity> getEntityAt(int x, int y) const;
    void removeEntity(int x, int y);
    void printNaturalMap() const;
    NaturalElement* getNaturalElementAt(int x, int y) const;
    void removeNatural(int x, int y);
    bool isAdjacentToMate(int x, int y, char symbol) const;
    void mateEntities(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);
    void transformMineralToGrass(int x, int y);
    bool hasLivingEntities() const;
    void printEntityCounts() const;
    std::pair<int, int>getEntityCounts() const;
};
