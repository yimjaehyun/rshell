#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <sys/wait.h>
#include <sys/types.h>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
using namespace std;

class Executable {
    public:
        Executable(){}; //Constructor
        Executable(const string &s) //constructor that takes in a string parameter
        {
            exec = s;
        };
	virtual ~Executable() {} //Destructor
        
        virtual bool run() { // Executes the function 
            stringstream s2;	//Stringstream separates strings into commands and arguments
            s2.str(exec); 
            
            vector<string> a;  
            string cmd, tmp;
            s2 >> cmd;
            a.push_back(cmd);
            while (s2 >> tmp) {
            	a.push_back(tmp);
            }
            //Turns array of strings into an array of Cstrings
            char ** argv = new char * [a.size() + 1];
            for (unsigned i = 0; i < a.size(); i++) {
            	argv[i] = (char *)(a[i].c_str());
            }
            argv[a.size()] = NULL;
            
	    int err;
            pid_t pid = fork();
            if (cmd == "exit") {
                exit(0);//If the command succeeded then return true
            }
            else if (pid < 0) {
            	perror("error: fork failed");
            	return false;
            }
            else if (pid == 0) {
            	execvp(cmd.c_str(), argv);
            	perror(exec.c_str());
		exit(1); //If the command failed then return fail
            	return false;
            }
            else {
            	if (waitpid(pid, &err, 0) < 0) {
            		perror("error: unable to wait");
            		return false; //Returns false if there were errors in running the command
            	}
                return err == 0;
            }
            return true;

        };
    
    public:
        string exec; //Holds the executable
};

#endif
