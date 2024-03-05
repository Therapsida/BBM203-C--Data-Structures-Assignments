#include "Leaderboard.h"



void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    
    LeaderboardEntry* temp = head_leaderboard_entry;
    LeaderboardEntry* prev_temp = nullptr;

    if(head_leaderboard_entry == nullptr)
    {
        head_leaderboard_entry = new_entry;
        return;
    }

    while (temp != nullptr)
    {
	    if(new_entry->score > head_leaderboard_entry->score)
	    {
            new_entry->next_leaderboard_entry = head_leaderboard_entry;
            head_leaderboard_entry = new_entry;
	    }
        else if(prev_temp == nullptr && head_leaderboard_entry->next_leaderboard_entry != nullptr)
        {
            temp = head_leaderboard_entry->next_leaderboard_entry;
        	prev_temp = head_leaderboard_entry;
        }
        else
        {
	        if(new_entry->score > temp->score){
                prev_temp->next_leaderboard_entry = new_entry;
                new_entry->next_leaderboard_entry = temp;
                break;
	        }
            if (temp->next_leaderboard_entry == nullptr)
            {
                temp->next_leaderboard_entry = new_entry;
                break;
            }
	        
        	prev_temp = temp;
        	temp = temp->next_leaderboard_entry;
        }
    }

    removeDuplicates();

    temp = head_leaderboard_entry;
    int x = 0;
    if(head_leaderboard_entry != nullptr){
	    while (temp->next_leaderboard_entry != nullptr)
	    {
    		x++;
	        if(x >= 10)
	        {
	            delete temp->next_leaderboard_entry;
	            temp->next_leaderboard_entry = nullptr;
	            break;
	        }
	        else
	        {
	            temp = temp->next_leaderboard_entry;
	        }
	    }
    }
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
 
}

void Leaderboard::write_to_file(const string& filename) {
    
    std::ofstream file(filename);
    file.flush();
    LeaderboardEntry* temp = head_leaderboard_entry;
	while (temp != nullptr)
    {
		file << temp->score << " " << temp->last_played <<" "<< temp->player_name << std::endl;
        temp = temp->next_leaderboard_entry;
    }

    file.close();
    
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
}

void Leaderboard::read_from_file(const string& filename) {
    
    std::ifstream file(filename);

    LeaderboardEntry* temp = head_leaderboard_entry;
    string line;
    while(std::getline(file,line)){
        std::stringstream ss(line);
        string part;
        string array[3];
        for (int i = 0; i < 3; ++i)
        {
            ss >> part;
            array[i] = part;
        }
        LeaderboardEntry* new_leaderboard = new LeaderboardEntry(stoi(array[0]), stoi(array[1]), array[2]);

        if(temp == nullptr)
        {
            head_leaderboard_entry = new_leaderboard;
            temp = head_leaderboard_entry;
        }
        else
        {
            temp->next_leaderboard_entry = new_leaderboard;
            temp = new_leaderboard;
        }
    }
    file.close();


    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
}


void Leaderboard::print_leaderboard() {
    
    std::cout << "Leaderboard\n-----------";
    int number = 1;
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp != nullptr)
    {
        std::cout << std::endl;


        char buffer[80];
        strftime(buffer, 80, "%H:%M:%S/%d.%m.%Y", localtime(&temp->last_played));

        std::cout << number++ << ". " << temp->player_name << " " << temp->score << " " << buffer;
        temp = temp->next_leaderboard_entry;
    }

    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    
}


Leaderboard::~Leaderboard() {
    
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (head_leaderboard_entry != nullptr)
    {
        temp = head_leaderboard_entry;
        head_leaderboard_entry = head_leaderboard_entry->next_leaderboard_entry;
        delete temp;
    }
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    
}


void Leaderboard::removeDuplicates()
{
    if(head_leaderboard_entry == nullptr)
        return;


    LeaderboardEntry* firstOcc = head_leaderboard_entry;
    while (firstOcc->next_leaderboard_entry != nullptr)
    {
        LeaderboardEntry* secOcc = firstOcc->next_leaderboard_entry;
        LeaderboardEntry* prev_secOcc = firstOcc;
        while (secOcc != nullptr)
        {
            if(secOcc->player_name == firstOcc->player_name)
            {
                prev_secOcc->next_leaderboard_entry = secOcc->next_leaderboard_entry;
                delete secOcc;
                secOcc = prev_secOcc->next_leaderboard_entry;
            }
            else
            {
                prev_secOcc = secOcc;
                secOcc = secOcc->next_leaderboard_entry;
            }


        }
        if(firstOcc->next_leaderboard_entry != nullptr)
			firstOcc = firstOcc->next_leaderboard_entry;
        else
        {
	        break;
        }
    }
		


}
