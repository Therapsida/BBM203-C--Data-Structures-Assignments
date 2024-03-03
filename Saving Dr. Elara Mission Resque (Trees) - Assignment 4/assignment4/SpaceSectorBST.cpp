#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {

    freeTree(root);



    // Free any dynamically allocated memory in this class.
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file,line))
    {
        std::stringstream ss(line);
        std::string  x,y,z;
        getline(ss, x, ',');
        getline(ss, y, ',');
        getline(ss, z, ',');

        Sector* new_sector = new Sector(stoi(x), stoi(y), stoi(z));

        if (root == nullptr) {
            root = new_sector;
        }
        else
        {
        	insertNode(root, new_sector);
        }
        
    }




    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {

    Sector* new_sector = new Sector(x, y, z);
    if (root == nullptr) {
        root = new_sector;
    }
    else
    {
        insertNode(root, new_sector);
    }

    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {

	 deleteItem(root, sector_code);
    // printBT("", root, false);
    // TODO: Delete the sector given by its sector_code from the BST.
}

void SpaceSectorBST::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    printInOrder(root);


    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
}

void SpaceSectorBST::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    printPreOrder(root);
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorBST::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    printPostOrder(root);
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;

	Sector* path_node = root;


    insertPath(path_node,path,sector_code);

    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {

    if (path.empty()){
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        return;
    }


    std::cout << "The stellar path to Dr. Elara: " << path[0]->sector_code;


    for (int i = 1; i < path.size(); ++i)
    {
        std::cout << "->"  << path[i]->sector_code ;
    }

    std::cout << std::endl;
    


    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}



void SpaceSectorBST::insertNode(Sector* parent, Sector* newNode)
{
	if(*newNode < *parent)
    {
        if (parent->left == nullptr){
            parent->left = newNode;
            newNode->parent = parent;
        }
        else
        {
            insertNode(parent->left, newNode);
        }

    }
	else if(*parent == *newNode)
	{
        return;
	}
    else{
	    if(parent->right == nullptr)
	    {
	        parent->right = newNode;
	        newNode->parent = parent;
	    }
	    else
	    {
	        insertNode(parent->right, newNode);
	    }
    }



}

void SpaceSectorBST::deleteItem(Sector* node, const  std::string& sector_code)
{

    if (node == nullptr)
        return;

    if (node->sector_code == sector_code) {
        deleteNode(node);
        return;
    }

	deleteItem(node->left,sector_code);
	deleteItem(node->right, sector_code);
    
    
   



}


void SpaceSectorBST::deleteNode(Sector* deleteNode)
{

	Sector* parent = deleteNode->parent;

	if(deleteNode->right == nullptr && deleteNode->left == nullptr)
    {

        if (root == deleteNode){
            root = nullptr;
            delete deleteNode;
            return;
        }

        if (parent->left == deleteNode)
            parent->left = nullptr;
        else
            parent->right = nullptr;
        
        delete deleteNode;

    }
	else if(deleteNode->right == nullptr){

        Sector* left = deleteNode->left;

        if (root == deleteNode){
            root = left;
            delete deleteNode;
            return;
        }

        if (parent->left == deleteNode)
            parent->left = left;
        else 
            parent->right = left;
        
        left->parent = parent;
        

		delete deleteNode;


    }
    else if (deleteNode->left == nullptr) {

        Sector* right = deleteNode->right;

        if (root == deleteNode) {
            root = right;
            delete deleteNode;
            return;
        }

        if (parent->left == deleteNode) 
            parent->left = right;
        else 
            parent->right = right;
        

        right->parent = parent;

        

        delete deleteNode;
    }
	else
        
	{
        Sector* left_most = findLeftMost(deleteNode->right);

       if(deleteNode->right == left_most)
       {
           
           deleteNode->swap(*left_most);
           deleteNode->right = left_most->right;
           if(left_most->right != nullptr)
           {
               deleteNode->right->parent = deleteNode;
           }


           delete left_most;
       }

        else{
	        deleteNode->swap(*left_most);



	        if(left_most->right != nullptr )
	        {
	                left_most->parent->left = left_most->right;
                    left_most->right->parent = left_most->parent;
	        }
	        
	        else
	        {
	        	left_most->parent->left = nullptr;
	            
	        }

            delete left_most;
        }
	}
    
    
}


Sector* SpaceSectorBST::findLeftMost(Sector* node)
{
    if(node->left == nullptr)
    {
        return node;
    }
	return findLeftMost(node->left);


}


void SpaceSectorBST::printPreOrder(Sector* node)
{
    if (node != nullptr) {
        std::cout << node->sector_code << std::endl;
        printPreOrder(node->left);
        printPreOrder(node->right);
    }
}

void SpaceSectorBST::printInOrder(Sector* node)
{
    if(node != nullptr){
	    printInOrder(node->left);
	    std::cout << node->sector_code << std::endl;
	    printInOrder(node->right);
    }
}
void SpaceSectorBST::printPostOrder(Sector* node)
{
    if (node != nullptr) {
        printPostOrder(node->left);
        printPostOrder(node->right);
        std::cout << node->sector_code << std::endl;
    }

}


bool SpaceSectorBST::insertPath(Sector* node, std::vector<Sector*> &path, const std::string& sector_code)
{
    if (node == nullptr)
        return false;

    if (node->sector_code == sector_code)
    {
        path.push_back(node);
        return true;
    }

    path.push_back(node);

    bool left = insertPath(node->left, path, sector_code);
    bool right = insertPath(node->right, path, sector_code);

    if (left || right){
        
        return true;
    }

	path.pop_back();
	return false;
}


void SpaceSectorBST::freeTree(Sector* node)
{
    if (node == nullptr)
        return;

    freeTree(node->left);
    freeTree(node->right);
    delete node;


}

