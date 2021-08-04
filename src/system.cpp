#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include  "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::string;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
   vector<int> pids=LinuxParser::Pids();
   processes_.clear();
    for (int pid :pids)
    {
        Process process(pid);
        if (process.Command().length() >1 && process.CpuUtilization() > 0.0 && process.Ram().length()>1)
        {
            processes_.push_back(process);
        }   
        
    }
    
    std::sort(processes_.begin(),processes_.end()); 
    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }