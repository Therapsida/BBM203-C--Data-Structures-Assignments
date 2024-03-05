#include "GameController.h"
#include <sstream>
#include <iostream>
#include <fstream>

bool GameController::play(BlockFall& game, const string& commands_file){

	LeaderboardEntry* entry = new LeaderboardEntry(game.current_score, game.curr_time, game.player_name);

    std::ifstream file(commands_file);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return false;
    }
    std::string line;

    int initial_location(0);
    while(game.active_rotation != nullptr)
    {

        if(!std::getline(file, line)){

            std::cout << "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n\n";
            std::cout << "Score: " << game.current_score << std::endl;
            int high_score;
        	if(game.leaderboard.head_leaderboard_entry != nullptr){
        		 high_score = game.current_score > game.leaderboard.head_leaderboard_entry->score ? game.current_score : game.leaderboard.head_leaderboard_entry->score;
            }
            else
            {
                 high_score = game.current_score;
            }
        	std::cout << "High Score: " << high_score << std::endl;
            print_grid(false, game);
            entry->score = game.current_score;
            game.leaderboard.insert_new_entry(entry);
            game.leaderboard.print_leaderboard();
            game.leaderboard.write_to_file(game.leaderboard_file_name);
            file.close();
            return true;
        }


        if (line == "MOVE_RIGHT")
        {
            if (game.active_rotation->shape[0].size() + initial_location < game.cols)
                
				initial_location++;
				if (!check_for_collisions(game, initial_location))
					initial_location--;

        }

        else if (line == "MOVE_LEFT") {
            if (initial_location != 0){
				initial_location--;
				if (!check_for_collisions(game, initial_location))
					initial_location++;
            }
        }
        else if (line == "ROTATE_RIGHT") {
            game.active_rotation = game.active_rotation->right_rotation;
            if (game.active_rotation->shape[0].size() + initial_location > game.cols)
                game.active_rotation = game.active_rotation->left_rotation;

            if (!check_for_collisions(game, initial_location))
            {
                game.active_rotation = game.active_rotation->left_rotation;
            }
        }
        else if (line == "ROTATE_LEFT") {
            game.active_rotation = game.active_rotation->left_rotation;
            if (!check_for_collisions(game, initial_location))
                game.active_rotation = game.active_rotation->right_rotation;
        }

        else if (line == "PRINT_GRID") {
            
            std::cout << "Score: "  << game.current_score << std::endl;
            int high_score;
            if (game.leaderboard.head_leaderboard_entry != nullptr) {
                high_score = game.current_score > game.leaderboard.head_leaderboard_entry->score ? game.current_score : game.leaderboard.head_leaderboard_entry->score;
            }
            else
            {
                high_score = game.current_score;
            }
            std::cout << "High Score: " << high_score << std::endl;
            print_grid(true,game,initial_location);
            std::cout << std::endl;
            
            

            
        }
        else if (line == "GRAVITY_SWITCH") {
            game.gravity_mode_on = game.gravity_mode_on ? 0 : 1;
        }

        else if (line == "DROP") {
            int first_occupied_row = true_location_finder(game, initial_location);
            int total_block = 0;

            for (int i = 0; i < game.active_rotation->shape.size(); ++i)
            {
                for (int j = 0; j < game.active_rotation->shape[0].size(); ++j)
                {
                    
                    
                    if(game.active_rotation->shape[i][j]){
                        total_block++;
						game.grid[first_occupied_row + i][j + initial_location] = 1;
                    }
                }
            }
           
            game.current_score += total_block * (first_occupied_row);
            game.total_blocks += total_block;


            initial_location = 0;
            game.active_rotation = game.active_rotation->next_block;



            if(true_location_finder(game,initial_location) == -1)
            {
                std::cout << "GAME OVER!\nNext block that couldn't fit:\n\n";
                for (int i = 0; i < game.active_rotation->shape.size(); ++i)
                {
                    for (int j = 0; j < game.active_rotation->shape[0].size(); ++j)
                    {
                        (game.active_rotation->shape[i][j]) ? std::cout << occupiedCellChar : std::cout << unoccupiedCellChar;
                    }
                    
                    std::cout << std::endl;
                }
                
                std::cout << std::endl << "Final grid and score:\n\n";
                std::cout << "Score: " << game.current_score << std::endl;
                int high_score;
            	if (game.leaderboard.head_leaderboard_entry != nullptr) {
                    high_score = game.current_score > game.leaderboard.head_leaderboard_entry->score ? game.current_score : game.leaderboard.head_leaderboard_entry->score;
                }
                else
                {
                    high_score = game.current_score;
                }
                std::cout << "High Score: " << high_score << std::endl;
                print_grid(false, game);
                file.close();
                entry->score = game.current_score;
                game.leaderboard.insert_new_entry(entry);
                game.leaderboard.print_leaderboard();
                game.leaderboard.write_to_file(game.leaderboard_file_name);
            	return false;
            }

        }

        gravity(game);

        check_power_up(game);

        check_row(game);

       

        /*
        
        

        
        */
		}



        std::cout << "YOU WIN!\nNo more blocks.\nFinal grid and score:\n\n";
        std::cout << "Score: " << game.current_score << std::endl;
        int high_score;
        if (game.leaderboard.head_leaderboard_entry != nullptr) {
            high_score = game.current_score > game.leaderboard.head_leaderboard_entry->score ? game.current_score : game.leaderboard.head_leaderboard_entry->score;
        }
        else
        {
            high_score = game.current_score;
        }
        std::cout << "High Score: " << high_score << std::endl;
        print_grid(false, game, initial_location);
        entry->score = game.current_score;
        game.leaderboard.insert_new_entry(entry);
		game.leaderboard.print_leaderboard();
        game.leaderboard.write_to_file(game.leaderboard_file_name);
        

    

        

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    file.close();
    return true;
}



bool GameController::check_for_collisions(BlockFall& game, int initial_location, int row )
{
	for (int i = 0; i < game.active_rotation->shape.size(); ++i)
	{
		for (int j = 0; j < game.active_rotation->shape[0].size(); ++j)
		{
            if (game.grid[i + row][j + initial_location] == 1 && game.active_rotation->shape[i][j] == 1)
                return false;

		}
	}

    return true;

}

void GameController::gravity(BlockFall& game)
{
    if(!game.gravity_mode_on)
    {
	    return;
    }
    int temp = 0;
    for (int j = 0; j < game.grid[0].size(); j++)
    {
        for (int i = 0; i < game.grid.size(); ++i)
        {
            for (int k = 0; k < game.grid.size() - i - 1; ++k)
            {
                if (game.grid[k][j] > game.grid[k + 1][j]) {
                    temp = game.grid[k + 1][j];
                    game.grid[k + 1][j] = game.grid[k][j];
                    game.grid[k][j] = temp;
                }
            }
        }
    }



}

void GameController::check_row(BlockFall& game)
{
    std::vector<int> completed_rows;
	for (int i = 0; i < game.rows; ++i)
	{
        int counter = 0;
		for (int j = 0; j < game.cols; ++j)
		{
            if (game.grid[i][j])
                counter++;
                
		}
        if(counter == game.cols){
            completed_rows.push_back(i);
        }
	}
    if(!completed_rows.empty())
    {
        std::cout << "Before clearing:\n";
        print_grid(false, game);
        std::cout << std::endl;
    }

    for (int i : completed_rows)
    {
	    game.grid.erase(game.grid.begin() + i);
	    game.current_score += game.cols;
        game.grid.insert(game.grid.begin(), std::vector<int>(game.cols, 0));
	    game.total_blocks -= game.cols;
        
        
	    
    }
    
    if (!completed_rows.empty()) {
        check_row(game);
        gravity(game);
    }
    


}

void GameController::check_power_up(BlockFall& game)
{
	for (int i = 0; i < game.rows - game.power_up.size() +1 ; ++i)
	{
		for (int j = 0; j < game.cols - game.power_up[0].size() + 1 ; ++j)
		{

            bool is_power_up = true;

			for (int k = 0; k < game.power_up.size() ; ++k)
			{
                bool is_row_true = true;

				for (int l = 0; l < game.power_up[0].size() ; ++l)
				{
                    if (static_cast<bool>(game.grid[i + k][j + l]) != game.power_up[k][l]){
                        is_row_true = false;
						break;
                    }
                    

				}
               
                if (!is_row_true){
                    is_power_up = false;
                    break;
                }
			}

            if (is_power_up)
            {
                // score.
                std::cout << "Before clearing:\n";
                print_grid(false, game);
                std::cout << std::endl;
                game.current_score += 1000 + game.total_blocks;
                game.total_blocks = 0;
                game.grid = std::vector<std::vector<int>>(game.rows, std::vector<int>(game.cols ,0));
            }


		}
	}






}

void GameController::print_grid(bool isBlockPrint, BlockFall& game, int initial_location)
{

    

    std::vector<std::pair<int, int>> locations;
    if(isBlockPrint){
	    
	    for (int i = 0; i < game.active_rotation->shape.size(); ++i)
	    {
	        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j)
	        {
	            locations.push_back(std::pair<int, int>(i, j + initial_location));
	        }
	    }

    }
    int occ = 0;
    for (int i = 0; i < game.grid.size(); ++i)
    {
        for (int j = 0; j < game.grid[0].size(); ++j)
        {
            if (isBlockPrint) {
                if (i == locations[occ].first && j == locations[occ].second)
                {
                    if (occ < locations.size() - 1)
                        occ++;
                    if (game.active_rotation->shape[i][j - initial_location])
                        std::cout << occupiedCellChar;

                    else
                    {
                        const char* cell = game.grid[i][j] ? occupiedCellChar : unoccupiedCellChar;
                        std::cout << cell;
                    }
                }
                else {
                    const char* cell = game.grid[i][j] ? occupiedCellChar : unoccupiedCellChar;
                    std::cout << cell;
                }
            }
            else
            {
                const char* cell = game.grid[i][j] ? occupiedCellChar : unoccupiedCellChar;
                std::cout << cell;
            }
        }
        std::cout << std::endl;
    }
    std::cout  << std::endl;
}




int GameController::true_location_finder(BlockFall& game,int initial_location)
{
    int first_occupied_row = 0;  //game.rows - game.active_rotation->shape.size()

    if (game.active_rotation == nullptr)
        return 1;


	int total_try = game.rows - game.active_rotation->shape.size();
    for (int i = 0; i < total_try + 1; ++i)
    {
        if (!check_for_collisions(game, initial_location, first_occupied_row))
        {
            break;
        }

        first_occupied_row++;
    }
    first_occupied_row--;

    if (first_occupied_row < 0)
    {
        return -1;
    }

    return first_occupied_row;
}


