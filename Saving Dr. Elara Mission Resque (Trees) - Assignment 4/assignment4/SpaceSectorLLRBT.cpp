#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string  x, y, z;
        getline(ss, x, ',');
        getline(ss, y, ',');
		getline(ss, z, ',');

        insertSectorByCoordinates(stoi(x), stoi(y), stoi(z));

    }
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {

    freeTree(root);

    // TODO: Free any dynamically allocated memory in this class.
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {

    Sector* node = new Sector(x, y, z);
    if (root == nullptr){
        root = node;
		root->color = BLACK;
    }
    else
    {
        insertNode(root, node);
    }


    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    printInOrder(root);
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {

    std::cout << "Space sectors preorder traversal:\n";
    printPreOrder(root);
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    printPostOrder(root);
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;

    Sector* path_node = root;

    Sector* earth = findEarth(root);

    insertPath(earth, path, sector_code);
    if (!path.empty())
    {
        return path;
    }


    int count = 0;

    while (path.size() == count)
    {
        path.push_back(earth);
        earth = earth->parent;
        insertPath(earth, path, sector_code);
        count++;

        if (earth == nullptr)
        {
            break;
        }

    }
    if (path.back()->sector_code != sector_code) {
        path.clear();
    }

        return path;




        // TODO: Find the path from the Earth to the destination sector given by its
        // sector_code, and return a vector of pointers to the Sector nodes that are on
        // the path. Make sure that there are no duplicate Sector nodes in the path!
        return path;
    

}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {

    if (path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        return;
    }


    std::cout << "The stellar path to Dr. Elara: " << path[0]->sector_code;


    for (int i = 1; i < path.size(); ++i)
    {
        std::cout << "->" << path[i]->sector_code;
    }

    std::cout << std::endl;

    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}


bool SpaceSectorLLRBT::insertPath(Sector* node, std::vector<Sector*>& path, const std::string& sector_code)
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
    if (left || right) {

        return true;
    }

    path.pop_back();
    return false;

}
void SpaceSectorLLRBT::printPreOrder(Sector* node)
{


    if (node != nullptr) {
        std::string color = node->color ? "RED sector: " : "BLACK sector: ";
        std::cout << color  << node->sector_code << std::endl;
        printPreOrder(node->left);
        printPreOrder(node->right);
    }
}

void SpaceSectorLLRBT::printInOrder(Sector* node)
{
    if (node != nullptr) {
        std::string color = node->color ? "RED sector: "   : "BLACK sector: ";
        printInOrder(node->left);
        std::cout << color << node->sector_code << std::endl;
        printInOrder(node->right);
    }
}
void SpaceSectorLLRBT::printPostOrder(Sector* node)
{
    if (node != nullptr) {
        std::string color = node->color ? "RED sector: " : "BLACK sector: ";
        printPostOrder(node->left);
        printPostOrder(node->right);
        std::cout << color << node->sector_code << std::endl;
    }

}


void SpaceSectorLLRBT::insertNode(Sector* sub_root , Sector* node)
{
    if(*node < *sub_root)
    {
        if(sub_root->left != nullptr)
			insertNode(sub_root->left, node);
        else
        {
            sub_root->left = node;
            node->parent = sub_root;
            arrangeBalance(sub_root);
        }
    }
    else if(*sub_root != *node)
    {
        if (sub_root->right != nullptr)
            insertNode(sub_root->right, node);
        else
        {
            sub_root->right = node;
            node->parent = sub_root;
            arrangeBalance(sub_root);
        }
    }


}

void SpaceSectorLLRBT::arrangeBalance(Sector* node)
{

    if (node == nullptr)
        return;

    if (node->right == nullptr && node->left == nullptr)
        arrangeBalance(node->parent);


    else if(node->left != nullptr && node->right != nullptr){
    if (node->left->color && node->right->color)
    {
        node->left->color = false;
        node->right->color = false;
        node->color = true;
        if (node == root)
            node->color = false;
        arrangeBalance(node->parent);

    }
    else if(node->color && node->left->color )
    {
        rightRotation(node->left, node, node->parent);
        arrangeBalance(node);
    }
    else if(node->right->color)
    {
        leftRotation(node->right, node, node->left);
        arrangeBalance(node);
    }
    }
    else if(node->left == nullptr)
    {
        if(node->right->color){
        leftRotation(node->right, node, node->left);
        arrangeBalance(node);
        }
    }
    else if(node->right == nullptr)
    {
	    if(node->color && node->left->color)
	    {
            rightRotation(node->left, node, node->parent);
            arrangeBalance(node);
	    }
    }
    


    arrangeBalance(node->parent);

}

void SpaceSectorLLRBT::rightRotation(Sector* left, Sector* node, Sector* parent)
{
    node->swap(*parent);

    node->left = node->right;

    node->right = parent->right;

	if (node->right != nullptr)
        node->right->parent = node;

	parent->right = parent->left;

    
    parent->left = left;
    left->parent = parent;




}

void SpaceSectorLLRBT::leftRotation(Sector* right, Sector* node, Sector* left)
{
    right->swap(*node);

    node->left = right;
    right->parent = node;

    node->right = right->right;
    if (node->right != nullptr)
        node->right->parent = node;


    right->right = right->left;

    right->left = left;
    if (left != nullptr)
        left->parent = right;




}

void SpaceSectorLLRBT::freeTree(Sector* node)
{
    if (node == nullptr)
        return;

    freeTree(node->left);
    freeTree(node->right);
    delete node;


}


Sector* SpaceSectorLLRBT::findEarth(Sector* root )
{
    
    if(root->x < 0)
    {
        return findEarth(root->right);
    }
    if(root->x > 0)
    {
        return findEarth(root->left);
    }
    else
    {
        if(root->y < 0)
        {
            return findEarth(root->right);
        }
        else if(root->y > 0 )
        {
            return findEarth(root->left);
        }
        else
        {
            if (root->z < 0)
            {
                return findEarth(root->right);
            }
            else if(root->z > 0)
            {
                return  findEarth(root->left);
            }
            else
            {
                return root;
            }

        }


    }



}