CXX = g++            # Le compilateur à utiliser
CXXFLAGS = -std=c++17 -Wall -g  # Les options de compilation, ajustez selon les besoins

# Fichiers objets
OBJS = main.o entity.o natural.o map.o wolf.o sheep.o mineral.o grass.o

# Nom de l'exécutable
EXEC = simulation

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

main.o: main.cpp map.h
	$(CXX) $(CXXFLAGS) -c main.cpp

entity.o: entity.cpp entity.h map.h
	$(CXX) $(CXXFLAGS) -c entity.cpp

natural.o: natural.cpp natural.h
	$(CXX) $(CXXFLAGS) -c natural.cpp

map.o: map.cpp map.h entity.h natural.h
	$(CXX) $(CXXFLAGS) -c map.cpp

clean:
	rm -f *.o $(EXEC)

.PHONY: all clean
