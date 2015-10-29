#include <iostream> 
#include <vector> 
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h> 

using namespace std; 
class ABC //Abstract Base Class
{
    public:
        virtual ~ABC()
        {} 
        virtual void execute() = 0; //pure virtual function
};

class Connectors: public ABC
{
    private:
        bool prevstate;  
        //Will hold value stating if prev state passed or failed 
    public: 
        void set_prevstate(bool prev)
        {
            prevstate = prev; 
        }
        bool get_prevstate()
        {
            return prevstate;
        }
        virtual void execute() 
        {}

}; 


class Semicolon_Connector: public Connectors
{
    private: 
        const char * argv[25];
           
    public:         
        Semicolon_Connector(bool prev, const vector<string> &args)
        {
            set_prevstate(prev);
                
            unsigned int i = 0; 
            for(i = 0; i < args.size(); i++)
            {
               //string cmd = args.at(i);
               argv[i] = args.at(i).c_str();
               //argv[i] = &cmd[i];  
            }
            argv[i] = NULL;  
        } 
        virtual void execute() 
        {
            //Always executes, no matter if the previous 
            //command failed or passed
            pid_t c_pid = fork(); //Child's process ID
            pid_t pid; //Parent's process ID  
            int status; //Will indicate the status of the child process 
            
            if(c_pid < 0) 
            {
               perror("Forking failed"); 
               set_prevstate(0);
               return;  
            }
            else if(c_pid == 0) 
            {
               //If fork() == 0, this fork is the child process
               execvp(argv[0], const_cast<char* const*>(argv)); 
               perror("execvp failed in child");
               //execvp, if successful, never returns
               set_prevstate(0); 
               return; 
            } 
            else 
            {
                if( (pid = waitpid(c_pid, &status, 0)) < 0) 
                { 
                    perror("ERROR");
                    //if waitpid returns -1, there was an ERROR 
                    set_prevstate(0); 
                    return; 
                }
                //else, child process if finished and the parent can continue
                //the rest of its process. 
                set_prevstate(1); 
                return; 
               //return 1 == Coommand has been executed
            }
         } 

};

class AND_Connector: public Connectors
{
    private: 
        const char * argv[25];

    public:
        AND_Connector(bool prev, const vector<string> & args)
        {
            set_prevstate(prev);
            unsigned int i = 0;   
            for(i = 0; i < args.size(); i++)
            {
               argv[i] = args.at(i).c_str();
               //string cmd = args.at(i); 
               //argv[i] = &args[i];  
            }
            argv[i] = NULL;  
        }
        virtual void execute()
        {
            if(get_prevstate() == 0)
                return; 
            else
           {
               pid_t c_pid = fork(); //Child's process ID
               pid_t pid; //Parent's process ID  
               int status; //Will indicate the status of the child process 
            
               if(c_pid < 0) 
               {
                   perror("Forking failed");
                   set_prevstate(0);    
                   return; 
                }   
               else if(c_pid == 0) 
               {
                   //If fork() == 0, this fork is the child process
                   execvp(argv[0], const_cast<char * const *>(argv)); 
                   perror("execvp failed in child");
                   set_prevstate(0); 
                   //execvp, if successful, never returns
               } 
               else 
               {
                   if((pid = waitpid(c_pid, &status, 0)) < 0) 
                   { 
                       perror("ERROR");
                       //if waitpid returns -1, there was an ERROR 
                       set_prevstate(0);
                       return;  
                   }
                   //else, child process if finished and the parent can continue
                   //the rest of its process. 
                   set_prevstate(1); 
                   return; 
                   //return 1 == Coommand has been executed
           }                 
        }
}; 

class OR_Connector: public Connectors
{
    private: 
        const char * argv[25]; 

    public:
        OR_Connector(bool prev, const vector<string> &args)
        {
            Connectors::set_prevstate(prev);
            unsigned int i = 0;  
            for(i = 0; i < args.size(); i++)
            {
              argv[i] = args.at(i).c_str(); 
              //string cmd = args.at(i); 
              //argv[i] = &args[i]; 
            }
            argv[i] = NULL;  
        } 
        virtual void execute()
        {
            if(get_prevstate() == 1)
                return; 
            else
            { 
                //Execute command (only when previous command fails
                pid_t c_pid = fork(); //Child's process ID
                pid_t pid; //Parent's process ID  
                int status; //Will indicate the status of the child process 
            
                if(c_pid < 0) 
                {
                    perror("Forking failed");
                    set_prevstate(0);    
                    return; 
                }   
                else if(c_pid == 0) 
                {
                    //If fork() == 0, this fork is the child process
                    execvp(argv[0], const_cast<char * const *>(argv)); 
                    perror("execvp failed in child");
                    set_prevstate(0); 
                    //execvp, if successful, never returns
                } 
                else 
                {
                    if((pid = waitpid(c_pid, &status, 0)) < 0) 
                    { 
                        perror("ERROR");
                        //if waitpid returns -1, there was an ERROR 
                        set_prevstate(0);
                        return;  
                    }
                    //else, child process if finished and the parent can continue
                    //the rest of its process. 
                    set_prevstate(1); 
                    return; 
                    //return 1 == Coommand has been executed
                } 
            }

        }
};
