#include "Network.h"
#include  <fstream>
#include <sstream>
Network::Network() : not_client("-1","-1","-1")
{

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                               const string &sender_port, const string &receiver_port) {

    this->clients = &clients;
    this->sender_port = sender_port;
    this->receiver_port = receiver_port;



	for (std::string&  command : commands)
	{
        string dashes(command.size()+9 ,'-' );

        std::cout << dashes << std::endl << "Command: " << command << "\n" << dashes << std::endl;

        std::string first_command;
        std::stringstream command_ss(command);
        command_ss >> first_command;



        if(first_command == "MESSAGE"){
	        string sender;
	        string reciever;
	        command_ss >> sender >> reciever;

            unsigned long long  message_start = command.find("#") + 1;
            unsigned long long message_end = command.find("#", message_start);
            string message = command.substr(message_start, message_end - message_start);
            std::cout << "Message to be sent: " << "\"" << message << "\"\n" << std::endl;

	        create_message( sender, reciever, message, message_limit);
        }
        else if(first_command == "SHOW_FRAME_INFO")
        {
            string client_ID;
            string queue_selection;
            int frame_number;

            command_ss >> client_ID;
            command_ss >> queue_selection;
            command_ss >> frame_number;

            show_info( client_ID, queue_selection,frame_number);
        }
        else if(first_command == "SHOW_Q_INFO")
        {
            string client_ID;
            string queue_selection;

            command_ss >> client_ID;
            command_ss >> queue_selection;

            show_info( client_ID, queue_selection);
        }
        else if(first_command == "SEND")
        {
            send();
        }
        else if (first_command == "RECEIVE")
        {
            receive();
        }
        else if (first_command == "PRINT_LOG")
        {
            string client_ID;
            command_ss >> client_ID;

            print_log(client_ID);
        }
        else
        {
            std::cout << "Invalid command." << std::endl;
        }

	}





    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return clients;
    }

    string total_lines;

    if (!std::getline(file, total_lines))
        return clients;


    std::string line;

    std::string ID_address;
    std::string MAC_address;
	std::string Client_ID;

    for (int i = 0; i < stoi(total_lines); ++i)
    {
        std::getline(file, line);
        std::stringstream ss(line);
        ss >> Client_ID;
        ss >> ID_address;
        ss >> MAC_address;

        clients.push_back(Client(Client_ID, ID_address, MAC_address));

    }



    // TODO: Read clients from the given input file and return a vector of Client instances.
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return ;
    }

    std::string destination;
    std::string nearest;

    std::string line;
    for (int i = 0; i < clients.size(); ++i)
	{
        while(std::getline(file, line) &&  line != "-"){
	        std::stringstream ss(line);
	        ss >> destination;
            ss >> nearest;
        	clients[i].routing_table.insert(std::make_pair(destination, nearest));
        }
        
	}



    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Can not open file!" << std::endl;
        return {};
    }

    string total_lines;

    if (!std::getline(file, total_lines))
        return {};


    std::string line;


    for (int i = 0; i < stoi(total_lines); ++i)
    {
        std::getline(file, line);
        commands.push_back(line);
    }


    // TODO: Read commands from the given input file and return them as a vector of strings.
    return commands;
}

void Network::create_message( const string& senderID ,const string& receiverID , string& message, int message_limit)
{
    Client& sender_c = find_client(senderID);
    Client& receiver_c = find_client(receiverID);

    if(sender_c.client_id == "-1 " || receiver_c.client_id == "-1")
    {
        std::cout << "Message can not be sent!" << std::endl;
        return;
    }

    if(find_client(sender_c.routing_table[receiverID]).client_id == "-1")
    {
        std::cout << "Message can not ve sent! No connection" << std::endl;
    }

    int total_frame = 1;
    for (size_t i = 0; i < message.size(); i += message_limit) {
        std::string message_part = message.substr(i, message_limit);
        std::cout << "Frame #" << total_frame << std::endl;
    	create_layers(sender_c, receiver_c, message_part);

        total_frame++;
    }

   // sender_c.messages.insert(std::pair<4>)



}

void Network::show_info( const string& client_ID,  const string& queue_selection, int frame_number)
{
    Client& client = find_client(client_ID);

    if(frame_number == -1)
    {
        string queue_type = queue_selection == "in" ? "Incoming" : "Outgoing";
        int total_frame = queue_selection == "in" ? client.incoming_queue.size() : client.outgoing_queue.size();

        std::cout << "Client " << client_ID << " " << queue_type << " Queue Status" << std::endl << "Current total number of frames: " << total_frame << std::endl;
    }
    else{

	    queue<stack<Packet*>> queue = queue_selection == "in" ? client.incoming_queue : client.outgoing_queue;

        string queue_type = queue_selection == "in" ? "incoming" : "outgoing";

        bool found = false;
        int size = queue.size();
		for (int i = 1; i < size+1 ; ++i)
		{
	        stack<Packet*> current_frame = queue.front();
	        queue.pop();
	        if(frame_number == i)
	        {
	        	found = true;
	            print_frame(current_frame,frame_number,queue_type,client_ID);
	        }
		}
        if(!found)
        {
            std::cout << "No such frame." << std::endl;
        }
    }
}

void Network::print_log( const string& client_ID)
{
	for (Client& client : *clients)
	{
        if(client_ID == client.client_id)
			client.print_log();
	}


}

void Network::send()
{
    for (Client& client : *clients)
    {
        while (!client.outgoing_queue.empty()) {

            client.send(*clients);
        }
    }
    
}

void Network::receive()
{
	for ( Client& client : *clients)
	{
		while(!client.incoming_queue.empty())
		{
            client.receive(*clients);
		}
	}




}

Client& Network::find_client(string id)
{
	for(Client& client  : *clients)
	{
        if (id == client.client_id)
            return client;

	}
    return not_client ;   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
}


void Network::create_layers(Client& sender_c, Client& receiver_c, const string& message){


	Client& client = find_client(sender_c.routing_table[receiver_c.client_id]);
	PhysicalLayerPacket* physical_layer = new PhysicalLayerPacket(3, sender_c.client_mac, client.client_mac);
	NetworkLayerPacket* network_layer = new NetworkLayerPacket(2, sender_c.client_ip, receiver_c.client_ip);
	TransportLayerPacket* transport_layer = new TransportLayerPacket(1, sender_port, receiver_port);
	ApplicationLayerPacket* application_layer = new  ApplicationLayerPacket(0, sender_c.client_id, receiver_c.client_id, message);

	stack<Packet*> frame;
	frame.push(application_layer);
	frame.push(transport_layer);
	frame.push(network_layer);
	frame.push(physical_layer);
	sender_c.outgoing_queue.push(frame);
    print_all(frame);
}

void Network::print_frame(stack<Packet*> frame, int frame_number , const string& queue_type, const string&  clientID)
{
    std::cout << "Current Frame #" << frame_number << " on the "<< queue_type <<" queue of client " << clientID << std::endl;
	PhysicalLayerPacket* layer_3 = static_cast<PhysicalLayerPacket*>(frame.top());
	frame.pop();
    NetworkLayerPacket* layer_2 = static_cast<NetworkLayerPacket*>(frame.top());
    frame.pop();
	TransportLayerPacket* layer_1 = static_cast<TransportLayerPacket*>(frame.top());
    frame.pop();
	ApplicationLayerPacket* layer_0 = static_cast<ApplicationLayerPacket*>(frame.top());
    frame.pop();
    std::cout << "Carried Message: " << "\"" << layer_0->message_data << "\"" << "\nLayer 0 info: Sender ID: " << layer_0->sender_ID << ", Receiver ID: " << layer_0->receiver_ID << std::endl;
    std::cout << "Layer 1 info: Sender port number: " << layer_1->sender_port_number << ", Receiver port number: " << layer_1->receiver_port_number << std::endl;
    std::cout << "Layer 2 info: Sender IP address: " << layer_2->sender_IP_address << ", Receiver IP address: " << layer_2->receiver_IP_address << std::endl;
    std::cout << "Layer 3 info: Sender MAC address: " << layer_3->sender_MAC_address << ", Receiver MAC address: " << layer_3->receiver_MAC_address << std::endl;
    std::cout << "Number of hops so far: " << layer_3->hop_value << std::endl;

}


void Network::print_all(stack<Packet*> frame)
{

    int hop_value = frame.top()->hop_value;
	for (int i = 0; i < 4; ++i)
	{
        frame.top()->print();
		frame.pop();
	}
    std::cout << "Number of hops so far: " << hop_value << std::endl << "--------\n";

}


Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
