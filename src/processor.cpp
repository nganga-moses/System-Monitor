#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float cpuUsage=0.0;

    auto util=LinuxParser::CpuUtilization();
     float user=stof(util[0]);
     float nice=stof(util[1]);
     float system=stof(util[2]);
     float idle=stof(util[3]);
     float iowait=stof(util[4]);
     float irq=stof(util[5]);
     float softirq=stof(util[6]);
     float steal=stof(util[7]);
    
    float totalIdle = idle + iowait;

    float nonIdle = user + nice + system + irq + softirq + steal;

    float totalUsage = totalIdle + nonIdle;

    cpuUsage = (totalUsage - totalIdle)/totalUsage;    
    

    return cpuUsage; 
    
    }
