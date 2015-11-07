// Melissa Castillo, mcast052 
// Jeremy Chan, jchan107
// CS100 ASSIGNMENT 1, main.cpp 
#include <iostream> 
#include <boost/tokenizer.hpp> 
#include <string>
#include <stdio.h>
#include "connections.h" 

int main()
{
    using namespace std; 
    using namespace boost;
    
    bool exitcheck = 0; 
    do
    {
        cout <<"$ ";
        string tkn_check;
        getline(cin,tkn_check);
        if(!tkn_check.empty())
        {
            //parses
            typedef tokenizer<char_separator<char> > tokenizer;
            char_separator<char> sep(" $", ";#");
            tokenizer tkn(tkn_check,sep);

            //Shows where it is parsed

            for(tokenizer::iterator iter = tkn.begin();iter != tkn.end();
                    ++iter)
            {
                cout << *iter << endl;   
            }

            //puts the parsed values into vectors by  command and connectors
            vector< vector<string> > commands;
            vector<string> indivCommand;

            tokenizer::iterator iter2;
            bool ifHash = false;
            for(iter2 = tkn.begin();iter2 != tkn.end();
                    ++iter2)
            {
                if(*iter2 == "#")
                {
                    commands.push_back(indivCommand); 
                    ifHash = true;
                    break;
                }
                if(*iter2 == ";" || *iter2 == "||" || *iter2 == "&&")
                {
                    if(indivCommand.size() != 0)
                    {
                         commands.push_back(indivCommand); //push into commands, a vector holding list of commands before connector
                    }
                    indivCommand.clear(); // clears vector holding list of commands
                    indivCommand.push_back(*iter2); // holds only the connector now
                    commands.push_back(indivCommand); // pushes single connector into commands
                    indivCommand.clear(); // clears vector again
                }
                else
                {
                    indivCommand.push_back(*iter2);
                }
            }
            //makes sure hash doesnt go into the commands vector
            if(!indivCommand.empty() && ifHash == false)
            {
                commands.push_back(indivCommand);
            }

            //outputs the vector

            for(unsigned int i = 0; i < commands.size(); i++)
            {
                cout << "Command at vector " << i << " contains: " ;
                for(unsigned int j = 0; j < commands.at(i).size();j++)
                {
                    cout << commands.at(i).at(j)<< " ";
                }
                cout << endl;
            }  

            //Vector that will hold objects of our class
            vector<Connectors *> args; 
            unsigned int j = commands.size() - 1;  
            bool connectorFront = false; 
            for(unsigned int i = 0; i < commands.size(); i++) 
                //Traverses through outer vector 
            { 
                if(i == 0 && commands.at(i).at(0) == ";") 
                {
                    perror("Syntax error near unexpected token ';'");
                    connectorFront = true;
                    break; 
                }
                else if(i == 0 && commands.at(i).at(0) == "||") 
                {
                    perror("Syntax error near unexpected token \"||\""); 
                    connectorFront = true;
                    break;
                }
                else if(i == 0 && commands.at(i).at(0) == "&&") 
                {
                    perror("Syntax error near unexpected token \"&&\""); 
                    connectorFront = true;
                    break;
                }

                //SPECIAL CASE: First command is always run
                else if(i == 0) 
                {
                    args.push_back(new Semicolon_Connector(0, commands.at(i) )); 
                    //args.push_back(obj);
                }
                else if(commands.at(i).at(0) == ";") 
                {
                    if(i == j)
                    {
                        break;
                    } 
                    //Gets vector<string> to the right
                    //Does not take the actual sign 
                    args.push_back(new Semicolon_Connector(0, commands.at(i + 1)));
                } 
                else if(commands.at(i).at(0) == "&&")
                { 
                    if(i == j) 
                    {
                        break; 
                    }
                    args.push_back(new AND_Connector(0, commands.at(i + 1) ));  
                }
                else if(commands.at(i).at(0) == "||")
                {
                    if(i == j) 
                    {
                        break; 
                    }
                    args.push_back(new OR_Connector (0, commands.at(i + 1) ));  
                }
            }    

            if(connectorFront == false)
            {
                unsigned int i = 0;  
                //Executes each command
                for(i = 0; i < args.size()-1; i++)
                {
                    //Dynamically calls appropriate execute() function
                    args.at(i)->execute();
                    //Variable holds whether the current command failed or succeeded
                    bool prev = args.at(i)->get_prevstate();
                    //Changes next command's bool to prev
                    args.at(i + 1)->set_prevstate(prev);
                    exitcheck = args.at(i)->get_exit(); 
                    if(exitcheck)
                    { 
                        break;
                    }  
                }
            // gets rid of out-of-bounds error
                args.at(i)->execute();
                if(!exitcheck)
                {
                    exitcheck = args.at(i)->get_exit(); 
                }

            //Deallocates objects and removes pointers
                for(unsigned int i = 0; i < args.size(); i++)
                { 
                    delete args[i]; 
                } 
                args.clear();
            }
        }
    } while(!exitcheck); 
    return 0; 
}     


