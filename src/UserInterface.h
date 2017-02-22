#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include "Executable.h"
#include "Parenthesis.h"
#include "Or.h"
#include "And.h"
#include "Connector.h"
#include "SemiColon.h"
#include "Test.h"
#include <unistd.h>
#include <cstdlib>
#include "Cd.h"
using namespace std;

class UserInterface
{
    public:
        UserInterface()
        {
            gethostname(hostname, 128);	//Gets hostname
            username = getlogin(); //Gets the username
        };
        
        void run()
        {
            string userInput;	//holds the userinput
            queue<Connector *> cons;	//Creates a queue of connectors
            queue<Executable *> exes;	//Create a queue full of executables 
            while(true) {
                cout << username << "@" << hostname << "$ ";	//Prints out hostname and login stuff
                getline(cin, userInput);

                parse(userInput, cons, exes);	//Parses what the uesr inputted and puts it into either cons or exes
                bool lastrun = true;
                while (!exes.empty()) {	//Executes all of the commands and connectors
                    Executable * top = exes.front();
                    if (cons.size() == exes.size()) { //if it is a connector do this
                        Connector * con = cons.front();
                        cons.pop();
                        lastrun = con->evaluate(lastrun);
                        delete con;
                    }
                    if (lastrun) {	//If true then make lastrun the top of run
                        lastrun = top->run();
                    }
                    exes.pop();
                    delete top;
                }
            }
        };
        
    private:
        void parse(const string &input, queue<Connector *> &cons, queue<Executable *> &exes) {	//implementation of parse everything
            int args = 0;
            int begin = 0;
            int end = 0;
            bool addpr = false;
            string holder;
            for(unsigned i = 0; i < input.size(); i++) {
        		if (input[i] == '#')	//If it is a # then ignore everything to the right of it
        		{
        		    if(i != 0)
        		    {
        		    	end = i;
                        add_exe(input.substr(begin, end - begin), exes);
        		    	i = input.size();
        		    }		
        		}
                else if(input[i] == '(')	//If there is a parenthesis then send it to the parenthesis class
                {
                    args++;
                    begin = ++i;
                    while(args != 0) {
                        if (input[i] == ')') {
                            if (--args == 0) {
                                end = i;
                                exes.push(new Parenthesis(input.substr(begin, end - begin)));
                            }
                        }
                        else if (input[i] == '(') {
                            args++;
                        }
                        i++;
                    }
                    begin = i+1;
                    addpr = true;
                }
                else if (input[i] == ';') {	//If it is a ; then continue on, it's always true
                    end = i;
                    if (!addpr) {
                        add_exe(input.substr(begin, end - begin), exes);
                    }
                    else {
                        addpr = false;
                    }
                    
                    if(i+1 != input.size())
                    {
                        cons.push(new SemiColon());
                    }
                    begin = end+1;
                    i++;
                }
                else if (input[i] == '&' && i+1 != input.size() && input[i+1] == '&') {	//If there is an and, check if the left is true then run the right side if it is
                    end = i;
                    if (!addpr) {
                        add_exe(input.substr(begin, end - begin), exes);
                    }
                    else {
                        addpr = false;
                    }
                    if(i+2 != input.size())
                    {
                        cons.push(new And());
                    }
                    begin = i+2;
                    i+=2;
                }
                else if (input[i] == '|' && i+1 != input.size() && input[i+1] == '|') {	//If there is a ||, check if the left is true and only run the right if it is false
                    end = i;
                    if (!addpr) {
                        add_exe(input.substr(begin, end - begin), exes);
                    }
                    else {
                        addpr = false;
                    }
                    if(i+2 != input.size())
                    {
                        cons.push(new Or());
                    }
                    begin = i+2;
                    i+=2;
                }
                else if (i == input.size()-1) {	//If we've reached the end, then add it to the queue
                    end = i+1;
                    if (!addpr) {
                        add_exe(input.substr(begin, end + 1 - begin), exes);
                    }
                    else {
                        std::cout << "error: something not in parentheses" << std::endl;
                        addpr = false;
                    }
                }
            }
        };

        void add_exe(const string &input, queue<Executable *> &exes) {	//checks to see if there is a test or [] in the cmd
            stringstream str(input);
            string cmd;
            str >> cmd;
            if (cmd == "test" || cmd == "[") {
                exes.push(new Test(input));
            }
            else if (cmd == "cd") {
                exes.push(new Cd(input));
            }
            else {
                exes.push(new Executable(input));
            }
        }

        char * username, hostname[128];
        
};

#endif
