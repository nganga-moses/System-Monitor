#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <format.h>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memTotal;
  float memFree;

  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
       if (key=="MemTotal:")
        {
          memTotal=stof(value);
        }else if(key=="MemFree:")
        {
          memFree=stof(value);
        }
      
    }
    
  }
  return ((memTotal-memFree)/memTotal); 
  }

long LinuxParser::UpTime() { 
  string line;
  string uptime;
  string idleTime;

  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> uptime >> idleTime;
  }
   

  return std::stol(uptime); 
}

long LinuxParser::Jiffies() { 
  
  return ActiveJiffies()+IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) { 
  string line,value;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      cpu.push_back(value);
    }
    
  }
  auto utime=stof(cpu[13]);
  auto stime= stof(cpu[14]);
  auto cutime= stof(cpu[15]);
  auto cstime= stof(cpu[16]);

  return utime + stime + cutime + cstime; 
}

long LinuxParser::ActiveJiffies() { 
  string key,value,line,user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >>guest >> guest_nice;
      if (key=="cpu"){
        return (stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal) + stol(guest) + stol(guest_nice));
        
      }  
      
    }
    
  }
  return 0; 
  }

long LinuxParser::IdleJiffies() { 
   string key,value,line,user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> user >> nice >> system >> idle >> iowait;
      if (key=="cpu"){
        return stol(iowait);
      }  
      
    }
    
  }
  return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  string key,value,line,value0,value1,value2,value3,value4,value5,value6,value7,value8,value9;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value0 >> value1 >> value2 >> value3 >> value4 >> value5 >> value6 >> value7 >> value8 >> value9;
      if (key=="cpu"){
        cpu.push_back(value0);
        cpu.push_back(value1);
        cpu.push_back(value2);
        cpu.push_back(value3);
        cpu.push_back(value4);
        cpu.push_back(value5);
        cpu.push_back(value6);
        cpu.push_back(value7);
        cpu.push_back(value8);
        cpu.push_back(value9);
        return cpu;
      }
     
      
    }
    
  }
  
  return cpu; 
}

int LinuxParser::TotalProcesses() { 
  string key,value,line,processes;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "processes")
      {
        return std::stoi(value);
      }
      
    }
      
  }
  return 0; 
  
  }

int LinuxParser::RunningProcesses() { 
  string key,value,line;

  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;

      if(key=="procs_running"){
        return std::stoi(value);
      }
    }
    
  }
  return 0; 
  
}

string LinuxParser::Command(int pid) { 
  string cmd,line;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if (filestream.is_open())
  {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> cmd;
  }
  
  return cmd; 
  
}

string LinuxParser::Ram(int pid) { 
  string key,size,line;

  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> size;

      if(key=="VmSize:"){
        return std::to_string(stoi(size)/1024);
      }
    }
    
  }
  return string(); 
}

string LinuxParser::Uid(int pid) { 
  string key,uid,line;

  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> uid;

      if(key=="Uid:"){
        return uid;
      }
    }
    
  }
  return string(); 
  }

string LinuxParser::User(int pid) { 
  string user,x,id,line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open())
  {
    while(std::getline(filestream,line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> id;
      if (id == Uid(pid))
      {
        return user;
      }
      
    }
    
  }
  
  return string(); }

long LinuxParser::UpTime(int pid) { 
  string key,value,line,starttime;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      cpu.push_back(value);
    }
    
  }
  
  auto starttimel=stol(cpu[21]);

  auto hertz = sysconf(_SC_CLK_TCK);
  auto seconds = starttimel/hertz;
  
  return seconds; 

 }

float LinuxParser::CpuUtilization(int pid) { 
  string key,value,line;
  vector<string> cpu;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      cpu.push_back(value);
    }
    
  }
  auto utime=stof(cpu[13]);
  auto stime= stof(cpu[14]);
  auto cutime= stof(cpu[15]);
  auto cstime= stof(cpu[16]);
  auto starttime=stof(cpu[21]);

  auto totaltime = utime + stime + cutime + cstime;
  auto hertz = sysconf(_SC_CLK_TCK);
  auto seconds = UpTime()-(starttime/hertz);
  
  return ((totaltime/hertz)/seconds); 
}
