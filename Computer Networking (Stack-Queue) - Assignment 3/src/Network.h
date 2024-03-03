#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();


    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client>& clients, vector<string>& commands, int message_limit, const string& sender_port,
        const string& receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const& filename);
    void read_routing_tables(vector<Client>& clients, string const& filename);
    vector<string> read_commands(const string& filename);

    void create_message(const string& sender, const string& receiver, string& message, int message_limit);
    void show_info(const string& client_ID, const string& queue_selection, int frame_number = -1);
    void print_log(const string& client_ID);
    void send();
    void receive();
    void create_layers(Client& sender_c, Client& receiver_c, const string& message);
    void print_frame(stack<Packet*> frame, int frame_number, const string& queue_type, const string& clientID);
    Client& find_client(string id);
    void static print_all(stack<Packet*> frame);

    Client not_client;
    vector<Client>* clients;
	string sender_port;
	string receiver_port;


};

#endif  // NETWORK_H
