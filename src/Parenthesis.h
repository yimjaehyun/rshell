#ifndef PARENTHESIS_H
#define PARENTHESIS_H

#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include "Executable.h"
#include "Or.h"
#include "And.h"
#include "Connector.h"
#include "SemiColon.h"
#include "Test.h"
#include "Cd.h"
using namespace std;

class Parenthesis : public Executable
{
	public:
		Parenthesis() {};	//Constructor 
		Parenthesis(const string &s)	//Constructor that takes in a string
		{
			exec = s;
		}
		bool run()
		{
            queue<Connector *> cons;	//Holds a queue of connectors
            queue<Executable *> exes;	//Holds a queue of executables

                parse(exec, cons, exes); //parses everything
                bool lastrun = true;
                while (!exes.empty()) {
                    Executable * top = exes.front();	//Sets top to the first executable
                    if (cons.size() == exes.size()) {	//Do this if the cons size and execs size is the same
                        Connector * con = cons.front();
                        cons.pop();
                        lastrun = con->evaluate(lastrun);
                        delete con;
                    }
                    if (lastrun) {
                        lastrun = top->run();			//If lastrun is true then set lastrun is equal to the top of run
                    }
                    exes.pop();
                    delete top;
                }
            return lastrun;
		}
	private:
		void parse(const string &input, queue<Connector *> &cons, queue<Executable *> &exes) {	//parses the connectors sand execs
            int args = 0;
            int begin = 0;
            int end = 0;
            bool addpr = false;
            string holder;
            for(unsigned i = 0; i < input.size(); i++) {
        		if (input[i] == '#')
        		{
        		    if(i != 0)
        		    {
        		    	end = i;
                        add_exe(input.substr(begin, end - begin), exes);
        		    	i = input.size();
        		    }		
        		}
                else if(input[i] == '(')
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
                else if (input[i] == ';') {	//If input is equal to ;, then come here
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
                else if (input[i] == '&' && i+1 != input.size() && input[i+1] == '&') {	//If input is equal to &, then come here
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
                else if (input[i] == '|' && i+1 != input.size() && input[i+1] == '|') {	//If input is equal to ||, then come here
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
                else if (i == input.size()-1) {
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

        void add_exe(const string &input, queue<Executable *> &exes) {	//Tests to see if the cmd is test
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

	protected:
		string exec;	//Creates a string that holds the current executable
};


#endif
