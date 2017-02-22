#ifndef OR_H
#define OR_H

#include "Connector.h"

class Or : public Connector
{
   public:
        Or(){} //Constructor
		//If the first command succeeded, don't do the next
        bool evaluate(bool check){
            return !check;
        };
};       

#endif

