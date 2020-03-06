#include <unistd.h>
#include <cctype>
#include <sstream>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p) : pid(p){}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization(){ 
    
    string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename;
    std::vector<std::string> values = LinuxParser::ParseFile(filename, 1, 14, 17);
    long utime = stoi(values[0]);
    long stime = stoi(values[1]);
    long cutime = stoi(values[2]);
    long cstime = stoi(values[3]);
    
    float totaltime = utime + stime + cutime + cstime;

    float cpu_usage = ((totaltime / sysconf(_SC_CLK_TCK)) / UpTime());

    return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    
    string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename;
    return LinuxParser::ParseFile(filename, 1);
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
   
    string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename;
    float RamKB = stof(LinuxParser::ParseFile(filename, "VmSize:"));
    return to_string(RamKB/1000).substr(0, 7); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    
    string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename;
    string uid = LinuxParser::ParseFile(filename, "Uid:");
    
    return LinuxParser::User(uid);
}

// TODO: Return the age of this process (in seconds)
float Process::UpTime() { 
    //take the total uptime of machine in seconds and subtract
    //starttime of process in seconds
    float systemuptime = LinuxParser::UpTime(); // in seconds
    float processstarttime = LinuxParser::UpTime(pid)/sysconf(_SC_CLK_TCK); //in seconds
    return systemuptime - processstarttime;
 }

void Process::sort(std::vector<Process>& processes){
    std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {return a < b;});
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process&  a) { 
   return (CpuUtilization() > a.CpuUtilization()) ? true : false;
}
/*
bool operator<(Process& a, Process& b) { 
    return (a.CpuUtilization() < b.CpuUtilization()) ? true : false;
}
*/
