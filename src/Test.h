#ifndef TEST_H
#define TEST_H

#include "Executable.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sstream>
using namespace std;

class Test : public Executable
{
	public:
		Test(){}	//Constructor
    	Test(const string &s)	//Constructor that takes a string parameter
     	{
        	exec = s;
     	};
     	bool run(){	//Runs the exec 
     		stringstream str(exec);
               string flag, path, cmd, discard;
               int args = 1;
               str >> cmd;
               while(str >> discard) {
                    args++;
               }
               if (cmd == "test" && args == 3){	//If the command is test then come here
                    str.str(string());
                    str.clear();
                    str.str(exec);
                    str >> cmd >> flag >> path;
               }
               else if (cmd == "[" && args == 4) { //If the command has a [ then come here 
                    str.str(string());
                    str.clear();
                    str.str(exec);
                    str >> cmd >> flag >> path >> discard;
                    if (discard != "]") {
                         cout<<"-rshell: [: missing ']'"<<endl;			//If one of the [] is missing then return an error
                         return false;
                    }
               }
               else {
                    cout<<"-rshell: test: unary operator expected"<<endl;	//If neither is there then return an error
                    return false;
               }

     		struct stat st;
     		stat(path.c_str(), &st);

     		if(flag != "-e" && flag != "-f" && flag != "-d") {
     			cout<<"-rshell: test: " << flag <<": unary operator expected"<<endl;	//If there are no flags then return an error
     			return false;
     		}

     		bool ret = 	(flag == "-e" && (S_ISDIR(st.st_mode) || S_ISREG(st.st_mode))) ||		
     					(flag == "-f" && S_ISREG(st.st_mode)) ||
     					(flag == "-d" && S_ISDIR(st.st_mode));
						//Selects sthe correct command based on what flag is used. 
     		string out = (ret) ? "(True)" : "(False)";
     		cout<<out<<endl;
     		return ret;
     	}
};
#endif
