#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    this->pid=pid;
    this->user=LinuxParser::User(pid);
    this->cpuUtilization=LinuxParser::CpuUtilization(pid);
    this->command=LinuxParser::Command(pid);
    this->ram=LinuxParser::Ram(pid);
    this->uptime=LinuxParser::UpTime(pid);
    
};

int Process::Pid() { return this->pid; }

float Process::CpuUtilization() { 
    
  return this->cpuUtilization;

}

string Process::Command() { 
     std::string cmd =this->command;
     //limit the string for nice display purposes 
     return cmd.substr(0,40)+"...";
}

string Process::Ram() { 
    return this->ram; }

string Process::User() { 
    
    return this->user; 
}

long int Process::UpTime() { 
    return this->uptime; 
}

bool Process::operator<(Process const& a) const { 
    return a.cpuUtilization < cpuUtilization; 
}