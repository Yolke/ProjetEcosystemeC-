#pragma once
#include <memory>

class Map; // Forward declaration

class Entity {
protected:
    int x, y;
    bool isAlive;
    int age;
    int hunger;
    char sex;
    const int maxAge;
    const int maxHunger;
    const int maturityAge;

public:
    Entity(int x, int y, char sex, int maxAge, int maxHunger, int maturityAge, int age = 0); // Nouveau constructeur avec âge initial
    virtual ~Entity();

    virtual void update(Map& map) = 0;
    virtual char getSymbol() const = 0;
    virtual bool shouldBeRemoved() const;

    int getX() const;
    int getY() const;
    char getSex() const;
    void setSex(char newSex);
    bool canMate() const;
    std::pair<int, int>isAdjacentToMate(Map& map, char symbol) const;

    void move(Map& map);
    void ageAndHunger();
};

