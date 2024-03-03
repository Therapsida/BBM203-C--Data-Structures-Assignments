//
// Created by alperen on 27.09.2023.
//

#include "Client.h"

#include "Network.h"

Client::Client(string const& _id, string const& _ip, string const& _mac) {
    client_id = _id;
    client_ip = _ip;
    client_mac = _mac;
}

ostream &operator<<(ostream &os, const Client &client) {
    os << "client_id: " << client.client_id << " client_ip: " << client.client_ip << " client_mac: "
       << client.client_mac << endl;
	
    return os;
}


void Client::print_log()
{
    int log_number = 1;

    if (!log_entries.empty())
        std::cout << "Client " << client_id << " Logs:\n";

	for (Log log : log_entries)
	{
        string activity;
        
		switch (log.activity_type)
		{
        case ActivityType::MESSAGE_FORWARDED: activity = "Message Forwarded";
            break;
        case ActivityType::MESSAGE_DROPPED: activity = "Message Dropped";
            break;
        case ActivityType::MESSAGE_RECEIVED: activity = "Message Received";
            break;
        case ActivityType::MESSAGE_SENT: activity = "Message Sent";
            break;
		}
       
        std::cout << "--------------\n";
        std::cout << "Log Entry #" << log_number++ << ":\n" << "Activity: " << activity << std::endl;
        std::cout << "Timestamp: " << log.timestamp << std::endl;
		std::cout << "Number of frames: " << log.number_of_frames << std::endl;
        std::cout << "Number of hops: " << log.number_of_hops << std::endl;
        std::cout << "Sender ID: " << log.sender_id << std::endl;
        std::cout << "Receiver ID: " << log.receiver_id << std::endl << "Success: " << (log.success_status ? "Yes" : "No") << std::endl; 

        if (log.activity_type == ActivityType::MESSAGE_SENT || log.activity_type == ActivityType::MESSAGE_RECEIVED)
            std::cout << "Message: " << "\"" << log.message_content << "\"" << std::endl;
		
	}



}

void Client::send(vector<Client>& clients)
{
    if(outgoing_queue.empty())
        return;

    bool is_forwarded = false;
    if(outgoing_queue.front().top()->activity == ActivityType::MESSAGE_FORWARDED)
        is_forwarded = true;
    Client* receiver = nullptr;
    string message;
    int frame_number = 0;
    int hop_number = 0;
    bool is_find = false;
    bool message_sent = false;
    string receiverID = "None";
	while (!message_sent)
    {
        stack<Packet*>& frame = outgoing_queue.front();
		hop_number = frame.top()->hop_value;
        frame.top()->activity = ActivityType::MESSAGE_SENT;
		string message_part = get_message(frame);

		if((message_part.find('.')) != std::string::npos || (message_part.find('?')) != std::string::npos || (message_part.find('!')) != std::string::npos){
            message_sent = true;
        }

		message += message_part;

		is_find = find_receiver_mac_and_id(frame, clients, receiver, &receiverID);

		if(is_find){

            frame.top()->hop_value++;
            std::cout << "Client " << client_id << " sending frame #" << ++frame_number << " to client " << receiver->client_id << std::endl;
			Network::print_all(frame);

			
	        receiver->incoming_queue.push(frame);
	        outgoing_queue.pop();
        }
		else
		{
            free_frame(&frame);
            outgoing_queue.pop();
		}
    
        
    }

    

	if(!is_forwarded){

	    if(is_find)
	    {



	        Log log(current_timestamp(), message, frame_number, hop_number, client_id, receiverID, true, ActivityType::MESSAGE_SENT);
    		log_entries.push_back(log);
	    }
	    else
	    {
	        Log log(current_timestamp(), "None", 0, 0, client_id, "-1", false, ActivityType::MESSAGE_DROPPED);
	        log_entries.push_back(log);
	    }

    }
	


}


void Client::receive(vector<Client>& clients)
{
    
    if (incoming_queue.empty()){
        return;
    }
    stack<Packet*> find_receive = incoming_queue.front();

    PhysicalLayerPacket* P_layer_message = dynamic_cast<PhysicalLayerPacket*>(find_receive.top());

    Client* sender;
	for (Client& client : clients)
    {
        if (client.client_mac == P_layer_message->sender_MAC_address)
        {
            sender = &client;
        }
    }

	for (int i = 0; i < 3; ++i)
    {
        find_receive.pop();
    }
	ApplicationLayerPacket* first_occ = dynamic_cast<ApplicationLayerPacket*>(find_receive.top());

    string intended_client_id = first_occ->receiver_ID;
    string first_sender = first_occ->sender_ID;
	if(intended_client_id == client_id)
    {
        string message;
        int frame_number = 1;
        int hop_number = 0;
        bool is_find = false;
        bool message_received = false;
        string receiverID = "None";

        stack<Packet*> find_frame = incoming_queue.front();


		string sender_MAC = (dynamic_cast<PhysicalLayerPacket*>(find_frame.top())->sender_MAC_address);
        string senderID;
		for (Client& client : clients)
        {
            if (sender_MAC == client.client_mac) {
                senderID = client.client_id;
            }
        }

		for (int i = 0; i < 3; ++i)
        {
            find_frame.pop();

        }
        string origin_sender = dynamic_cast<ApplicationLayerPacket*>(find_frame.top())->sender_ID;


		while (!message_received)
        {
            stack<Packet*>& frame = incoming_queue.front();
            hop_number = frame.top()->hop_value;
            string message_part = get_message(frame);

            if ((message_part.find('.')) != std::string::npos || (message_part.find('?')) != std::string::npos || (message_part.find('!')) != std::string::npos) {
                message_received = true;
            }
            message += message_part;
            std::cout << "Client " << client_id << " receiving frame #" << frame_number << " from client " << senderID << ", originating from client " << origin_sender << std::endl;
            Network::print_all(frame);
            free_frame(&frame);
            incoming_queue.pop();
            frame_number++;
        }
        std::cout << "Client " << client_id << " received the message " << "\"" << message << "\" from client " << origin_sender << "." << std::endl << "--------\n";


         log_entries.push_back(Log(current_timestamp(), message, --frame_number, hop_number, origin_sender, client_id, true, ActivityType::MESSAGE_RECEIVED));

    }
    else
    {
        Client* destination_client = nullptr;
        string destination_id = routing_table[intended_client_id];
        for (Client& client : clients)
        {
            if(client.client_id == destination_id)
            {
                destination_client = &client;
            }

        }

    	int hop_number = dynamic_cast<PhysicalLayerPacket*>(incoming_queue.front().top())->hop_value;

		if(destination_client == nullptr)
		{

            string message;
            int frame_number = drop_message(&message);

            for (int i = 1; i < frame_number+1; ++i)
            {
                std::cout << "Client " << client_id << " receiving frame #" << i <<" from client "<< sender->client_id << ", but intended for client " << intended_client_id << 
                    ". Forwarding... \n"; 
                std::cout << "Error: Unreachable destination. Packets are dropped after " << hop_number << " hops!\n";
            }
            std::cout << "--------\n";

            log_entries.push_back(Log(current_timestamp(),message , frame_number, hop_number, first_sender, intended_client_id, false, ActivityType::MESSAGE_DROPPED));

		}
		else
		{
            bool forwarded = false;
            int frame_number = 1;

            
            std::cout << "Client " << client_id << " receiving a message from client " << sender->client_id << ", but intended for client " << intended_client_id << ". Forwarding... \n";

            string message;
            while(!forwarded){
                stack<Packet*>& curr_frame = incoming_queue.front();
                curr_frame.top()->activity = ActivityType::MESSAGE_FORWARDED;
            	PhysicalLayerPacket* physical_layer = dynamic_cast<PhysicalLayerPacket*>(curr_frame.top());
               
            	string message_part = get_message(curr_frame);
                if ((message_part.find('.')) != std::string::npos || (message_part.find('?')) != std::string::npos || (message_part.find('!')) != std::string::npos) {
                    forwarded = true;
                }

            	physical_layer->sender_MAC_address = client_mac;

            	physical_layer->receiver_MAC_address = destination_client->client_mac;

            	std::cout << "Frame #" << frame_number <<" MAC address change: New sender MAC " << client_mac <<", new receiver MAC " << destination_client->client_mac << std::endl;

                outgoing_queue.push(curr_frame);
                incoming_queue.pop();

                frame_number++;
                
                
            }
            std::cout << "--------\n";


            log_entries.push_back(Log(current_timestamp(), message, frame_number-1, hop_number, first_sender, intended_client_id, true, ActivityType::MESSAGE_FORWARDED));

		}
			




    }
    
}




bool Client::find_receiver_mac_and_id(stack<Packet*> frame, vector<Client>& clients , Client*& mac_receiver, string* receiverID)
{

    const PhysicalLayerPacket* const  P_layer = dynamic_cast<PhysicalLayerPacket*>(frame.top());
    for (int i = 0; i < 3; ++i)
    {
        frame.pop();
    }
    const ApplicationLayerPacket* const  A_layer  = dynamic_cast<ApplicationLayerPacket*>(frame.top());

    *receiverID = A_layer->receiver_ID;
    


	for (Client& client : clients)
	{
        if (client.client_mac == P_layer->receiver_MAC_address){
            mac_receiver = &client;
            return true;
        }
	}

    mac_receiver = nullptr;
    return false;

}

string Client::get_message(stack<Packet*> frame)
{
	for (int i = 0; i < 3; ++i)
	{
        frame.pop();
	}
    return dynamic_cast<ApplicationLayerPacket*>(frame.top())->message_data;

}

void Client::free_frame(stack<Packet*>* frame)
{
	for (int i = 0; i < 4; ++i)
	{
        delete frame->top();
        frame->pop();
	}
		
}


int Client::drop_message(string* message)
{
    bool dropped = false;
    int number_of_frames = 0;
	while (!dropped)
	{
        stack<Packet*>& frame = incoming_queue.front();
        string message_part = get_message(frame);
        *message += message_part;
		if ((message_part.find('.')) != std::string::npos || (message_part.find('?')) != std::string::npos || (message_part.find('!')) != std::string::npos) {
            dropped = true;
        }
        free_frame(&frame);
        incoming_queue.pop();
        number_of_frames++;



	}
    return  number_of_frames;
		







}

string Client::current_timestamp()
{
    time_t current_time = time(nullptr);
    char buffer[80];
    strftime(buffer, 80, "%Y-%M-%d %H:%M:%S", localtime(&current_time));
    return  buffer;
}



Client::~Client() {
    
	while (!outgoing_queue.empty())
	{
        stack<Packet*>& frame = outgoing_queue.front();
		for (int i = 0; i < 4; ++i)
		{
            delete frame.top();
            frame.pop();
		}
        outgoing_queue.pop();
	}
    while (!incoming_queue.empty())
    {
        stack<Packet*>& frame = incoming_queue.front();
        for (int i = 0; i < 4; ++i)
        {
            delete frame.top();
            frame.pop();
        }
        incoming_queue.pop();
    }
    

    // TODO: Free any dynamically allocated memory if necessary.
}