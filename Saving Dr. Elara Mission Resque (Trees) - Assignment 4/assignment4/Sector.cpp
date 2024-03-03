#include "Sector.h"
#include <cmath>
// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {

    distance_from_earth = (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
	const char c_x = x == 0 ? 'S' : x < 0 ? 'L' : 'R';
	const char c_y = y == 0 ? 'S' : y < 0 ? 'D' : 'U';
	const char c_z = z == 0 ? 'S' : z < 0 ? 'B' : 'F';

    sector_code = std::to_string(static_cast<int>(distance_from_earth)) + c_x + c_y + c_z;

        // TODO: Calculate the distance to the Earth, and generate the sector code
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    this->parent = other.parent;
    this->sector_code = other.sector_code;
    this->right = other.right;
    this->left = other.left;
    this->distance_from_earth = other.distance_from_earth;
    this->color = other.color;

    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator<(const Sector& other) const {
    if (this->x < other.x){
        return true;
    }
	if(this->x == other.x){
        if (this->y < other.y){
            return true;
        }
        if(this->y == other.y){
	        if (this->z < other.z)
	            return true;
        }
    }
    return false;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}


Sector& Sector::swap(Sector& other) {

    Sector temp = *this;


    this->x = other.x;
    this->y = other.y;
    this->z = other.z;


    this->sector_code = other.sector_code;
    this->distance_from_earth = other.distance_from_earth;

    //this->color = other.color;

    //other.color = temp.color;
    other.sector_code = temp.sector_code;
    other.distance_from_earth = temp.distance_from_earth;

    other.x = temp.x;
    other.y = temp.y;
    other.z = temp.z;


    return *this;
}