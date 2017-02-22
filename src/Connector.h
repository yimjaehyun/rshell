#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
using namespace std;

class Connector
{
    public:
        Connector() {}; //Constructor
        virtual bool evaluate(bool) = 0; //Virtual function, to be redefined within each connector subclass
        virtual ~Connector() {}; //Destructor
};

#endif

