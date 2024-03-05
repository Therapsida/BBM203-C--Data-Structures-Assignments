#include "BlockFall.h"

#include <fstream>
#include <iostream>

#include <sstream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
	                                                                                                                                                             gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
    curr_time = time(nullptr);
}

void BlockFall::read_blocks(const string &input_file) {

    std::ifstream file(input_file);
    if(!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return;
    }

    std::string line;

    bool endOfBlock = false;
    Block* current_block = nullptr;
    Block* prev_block = nullptr;
    std::vector<std::vector<bool>> shape;

		while (std::getline(file,line))
		{
            std::vector<bool> row;
            char c = '0';


            if(line.empty())
                continue;

			if(line[0] == '[' && line.back() == ']')
			{
                std::stringstream ss(line.substr(1, line.size()-2));
                while (ss >> c) {
                    row.push_back(c == '1');
                }
                
			}
            else if(line[0] == '[')
            {
                std::stringstream ss(line.substr(1, line.size()));
                while (ss >> c) {
                    row.push_back(c == '1');
                }
            }

            else if(line.back() == ']')
            {
                std::stringstream ss(line.substr(0, line.size()-1));
                while (ss >> c) {
                    row.push_back(c == '1');
                }
            }
            else
            {
                std::stringstream ss(line);
                while (ss >> c) {
                    row.push_back(c == '1');
                }
            }
           
            shape.push_back(row);
            if(line.back() == ']')
            {
                Block* block = new Block;
                block->shape = shape;


                shape.clear();


                if(initial_block == nullptr)
                {
                    initial_block = block;
                    active_rotation = initial_block;
                }
                if(prev_block == nullptr)
                {
                    prev_block = block;
                }
                else if(current_block == nullptr)
                {

                    prev_block->next_block = block;
                    current_block = block;
                }
                else
                {
                    current_block->next_block = block;
                    prev_block = current_block;
                    current_block = block;
                }

            }

		}

        
        prev_block->next_block = nullptr;
        power_up = current_block->shape;
        delete current_block;
        arrangeRotations();
        file.close();
    /*
        Block* pointer = initial_block;
    while (pointer != nullptr)
    {
	    for (auto unknown : pointer->shape)
	    {
		    for (auto xd : unknown)
		    {
                cout << xd << " ";
		    }
            std::cout << std::endl;
	    }
	    
        std::cout << std::endl << "-----------------------------------------------------------" << std::endl;
        pointer = pointer->next_block;
    }

    */
    

    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
}

void BlockFall::initialize_grid(const string &input_file) {
    
    std::ifstream file(input_file);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return;
    }

	std::string line;
    while (std::getline(file,line))
    {
        std::stringstream  ss(line);
        int bool_value;
        std::vector<int> row;
    	while(ss >> bool_value)
    	{
            row.push_back(bool_value);
            if (bool_value)
                total_blocks++;
    	}
        grid.push_back(row);
        
    }
    rows = grid.size();
    cols = grid[0].size();



    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
}


BlockFall::~BlockFall() {

    Block* temp = nullptr;
	while (initial_block != nullptr)
	{
        temp = initial_block;
        initial_block = initial_block->next_block;

        delete temp->right_rotation;
        delete temp->left_rotation->left_rotation;
        delete temp->left_rotation;
        delete temp;
	}



    // TODO: Free dynamically allocated memory used for storing game blocks
}




void BlockFall::arrangeRotations()
{
    Block* block = initial_block;

    while (block != nullptr)
    {
	    Block* current_block = block;
		for (int i = 0; i < 3; ++i)
		{
	        std::vector<std::vector<bool>> new_shape;
	        
	        for (int k = 0; k < current_block->shape[0].size(); ++k) {
	            std::vector<bool> new_row;
        		for (int j = current_block->shape.size()-1 ; j >= 0; --j)
	            {
	                new_row.push_back(current_block->shape[j][k]);
	            }
	            new_shape.push_back(new_row);
	        }
	        Block* rotated_block = new Block;

	        rotated_block->shape = new_shape;

	        rotated_block->left_rotation = current_block;

            rotated_block->next_block = current_block->next_block;

	        current_block->right_rotation = rotated_block;

	        current_block = rotated_block;

		}
	    current_block->right_rotation = block;
	    block->left_rotation = current_block;

        block  = block->next_block;

    }



}