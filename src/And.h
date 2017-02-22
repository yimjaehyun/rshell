#ifndef AND_H
#define AND_H

#include "Connector.h"

class And : public Connector
{
    public:
        And(){} //Constructor
		//Bool evaluate checks if the value returns true or false
        bool evaluate(bool check){
            return check;
        }
        
};       

#endif

