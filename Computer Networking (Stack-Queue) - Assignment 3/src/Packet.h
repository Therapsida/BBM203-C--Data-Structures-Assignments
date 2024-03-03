#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <iostream>

#include "Log.h"

enum class ActivityType;
using namespace std;

class Packet {
public:
    Packet(int layerID);
    virtual ~Packet();

    int layer_ID;
    int hop_value = 0;
    ActivityType activity = ActivityType::MESSAGE_SENT;
    friend ostream &operator<<(ostream &os, const Packet &packet);
    virtual void print() {};
};

#endif // PACKET_H
