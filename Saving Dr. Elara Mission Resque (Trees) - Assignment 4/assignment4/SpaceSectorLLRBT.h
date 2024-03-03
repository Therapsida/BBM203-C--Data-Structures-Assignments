#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

	void arrangeBalance(Sector* node);
	void rightRotation(Sector* left, Sector* node, Sector* parent);
    void leftRotation(Sector* right, Sector* node, Sector* parent);
    void insertNode(Sector* sub_root, Sector* node);


    bool insertPath(Sector* node, std::vector<Sector*>& path, const std::string& sector_code);
    void printInOrder(Sector* node);
    void printPreOrder(Sector* node);
    void printPostOrder(Sector* node);
    void freeTree(Sector* node);
    Sector* findEarth(Sector* root);
};

#endif // SPACESECTORLLRBT_H
