#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
	bool check_for_collisions(BlockFall& game, int initial_location, int row = 0);
	string game_status_information;
	void static gravity(BlockFall& game);
	void check_row(BlockFall& game);
	void check_power_up(BlockFall& game);
	void print_grid(bool isBlockPrint, BlockFall& game, int initial_location = 0);
	int true_location_finder(BlockFall& game, int initial_location);
};


	


#endif //PA2_GAMECONTROLLER_H
