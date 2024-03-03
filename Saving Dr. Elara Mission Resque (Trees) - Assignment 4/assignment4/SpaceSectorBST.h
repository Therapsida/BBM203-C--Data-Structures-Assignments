#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

private:
    void insertNode(Sector* parent, Sector* insertNode);
    void deleteNode(Sector* deleteNode);
    void deleteItem(Sector* node, const std::string& sector_code);
    Sector* findLeftMost(Sector* node);
	void printPreOrder(Sector* node);
    void printInOrder(Sector* node);
    void printPostOrder(Sector* node);
    bool insertPath(Sector* node, std::vector<Sector*>& path, const std::string& sector_code);
    void freeTree(Sector* node);

};

#endif // SPACESECTORBST_H
