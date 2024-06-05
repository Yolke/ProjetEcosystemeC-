#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream> 
#include "map.h"
#include "wolf.h"
#include "sheep.h"


int main() {
    srand(time(nullptr)); // Initialisation du générateur de nombres aléatoires

    std::cout << "Choisissez la taille de la carte (1: 5x5, 2: 10x10, 3: 15x15, 4: Par défaut 10x10 avec 50 moutons et 5 loups): ";
    int choice;
    std::cin >> choice;

    int width, height, numSheep, numWolves;
    switch(choice) {
        case 1:
            width = height = 5;
            std::cout << "Entrez le nombre de moutons: ";
            std::cin >> numSheep;
            std::cout << "Entrez le nombre de loups: ";
            std::cin >> numWolves;
            break;
        case 2:
            width = height = 10;
            std::cout << "Entrez le nombre de moutons: ";
            std::cin >> numSheep;
            std::cout << "Entrez le nombre de loups: ";
            std::cin >> numWolves;
            break;
        case 3:
            width = height = 15;
            std::cout << "Entrez le nombre de moutons: ";
            std::cin >> numSheep;
            std::cout << "Entrez le nombre de loups: ";
            std::cin >> numWolves;
            break;
        case 4:
            width = height = 10;
            numSheep = 50;
            numWolves = 5;
            break;
        default:
            std::cout << "Choix invalide, utilisation des paramètres par défaut: 10x10 avec 25 moutons et 15 loups." << std::endl;
            width = height = 10;
            numSheep = 50;
            numWolves = 5;
            break;
    }

    Map map(width, height); // Création de la carte

    if (numSheep + numWolves > width * height) {
        std::cout << "Trop d'entités pour la carte choisie! Réduisez le nombre d'entités." << std::endl;
        return 1; // Sortie précoce du programme en cas de surpopulation
    }

    // Ajouter des moutons à des positions aléatoires
    for (int i = 0; i < numSheep; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        while (!map.isCellEmpty(x, y)) {
            x = rand() % width;
            y = rand() % height;
        }
        char sex = (rand() % 2 == 0) ? 'M' : 'F';
        auto sheep = std::make_shared<Sheep>(x, y, sex, 5);
        map.addEntity(sheep, x, y);
    }

    // Ajouter des loups à des positions aléatoires
    for (int i = 0; i < numWolves; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        while (!map.isCellEmpty(x, y)) {
            x = rand() % width;
            y = rand() % height;
        }
        char sex = (rand() % 2 == 0) ? 'M' : 'F';
        auto wolf = std::make_shared<Wolf>(x, y, sex, 5);
        map.addEntity(wolf, x, y);
    }

    std::ofstream statsFile("entity_stats.csv");
    statsFile << "Tour,Loups,Moutons\n"; // En-tête du fichier CSV

    // Début de la simulation
    int turnCount = 0;
    while (map.hasLivingEntities()) {
        std::pair<int, int> counts = map.getEntityCounts();
        statsFile << turnCount << "," << counts.first << "," << counts.second << "\n";

        std::cout << "Tour " << ++turnCount << ":" << std::endl;
        map.update(); // Mise à jour de la carte
        map.printMap(); // Affichage de l'état de la carte
        map.printEntityCounts(); // Affiche le nombre de loups et de moutons
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause pour visualiser l'évolution
    }

    statsFile.close(); // Fermeture du fichier
    std::cout << "Fin de la simulation après " << turnCount << " tours.\n";
    return 0;
}
