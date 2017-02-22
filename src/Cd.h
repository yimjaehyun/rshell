#ifndef __CD_H__
#define __CD_H__

#include "Executable.h"
#include <unistd.h>
#include <string>
#include <sstream>
using namespace std;

class Cd : public Executable
{
	public:
		Cd(){}	//Constructor
    	     Cd(const string &s)	//Constructor that takes a string parameter
     	{
        	exec = s;
     	};
     	bool run(){	//Runs the exec 
               string flag;
     		stringstream sstr;
               sstr.str(exec);
               if (!(sstr >> flag >> flag)) {
                    flag = string(getenv("HOME"));
               }
               else if (flag == "-") {
                    flag = string(getenv("OLDPWD"));
               }

               char * oldpwd = getcwd(NULL, 0);
               int override = chdir(flag.c_str());
               char * pwd = getcwd(NULL, 0);
               setenv("OLDPWD", oldpwd, override + 1);
               setenv("PWD", pwd, override + 1);
               return override + 1;
     	}
};
#endif
