#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


std::string LinuxParser::ParseFile(std::string filename, int row, int col){
  std::ifstream file;
  string line;
  string target;
  file.open(filename);
  if (file.is_open()){
    
    //find the row
    for (int i = 0; i < row; i++){
      getline(file, line);
    }
    std::istringstream stream(line);
    //find the column
    for (int i = 0; i < col; i++){
      stream >> target;
    }
    //std::cout << "\n" + target;
    return target;

  } else{
    std::cout << "error\n";
  }
  file.close();
  return "not found";
}

std::string LinuxParser::ParseFile(std::string filename, std::string key){

  std::ifstream file;
  string line, header, target;

  file.open(filename);
  if (file.is_open()){
    while(getline(file, line)){
      std::istringstream stream(line);
      stream >> header;
      if (header == key){
        stream >> target;
        //std::cout << target << std::endl;
        file.close();
        return target;
      }  
    }
  } else{
    std::cout << "error\n";
  }
  file.close();
  return "not found";
}


std::string LinuxParser::ParseFile(std::string filename, int row){

  std::ifstream file(filename);
  string line;
  for (int i = 0; i < row; i++){
      getline(file, line);
  }
  return line;

}

std::vector<std::string> LinuxParser::ParseFile (std::string filename, int row, int col1, int col2){

  std::ifstream file(filename);
  string line;

  //find row
  for (int i = 0; i < row; i++){
      getline(file, line);
  }
  std::istringstream stream(line);

  //find column
  string skip;
  for (int i = 1; i < col1; i++){
    stream >> skip;
  }
  
  //get values
  int num = col2 - col1 + 1;
  string target;
  vector<string> targets;
  for (int i = 0; i < num; i++){
    stream >> target;
    targets.push_back(target);
  }

  std::reverse(targets.begin(), targets.end());
  return targets;

}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  
  string line, key, value;
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
  return LinuxParser::ParseFile(kProcDirectory + kVersionFilename, 1, 3);
}

// BONUS: Update this to use std::filesystem
// I think this code is right, did some testing on my machine. Doesn't build on VM (Older compiler version?).
/*
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  fs::path directory(kProcDirectory);
  
  for (auto& entry : fs::directory_iterator(directory)){
  
    if (fs::is_directory(entry.status())){
    
      string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
          
    }
  }
  return pids;
}
*/

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  //Read from /proc/meminfo MemFree: MemTotal: 
  float MemTotal = stof(ParseFile(kProcDirectory + kMeminfoFilename, "MemTotal:"));
  float MemFree = stof(ParseFile(kProcDirectory + kMeminfoFilename, "MemFree:"));
 
  return (MemTotal - MemFree)/MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  return stoi(LinuxParser::ParseFile(kProcDirectory + kUptimeFilename, 1, 1));
}

float LinuxParser::ProcessStartTime(int pid){
  //number of seconds that passed before process started
  float starttime = stof(ParseFile(kProcDirectory + to_string(pid) + kStatFilename, 1, 22))/sysconf(_SC_CLK_TCK);
  return starttime;

}


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::SystemCpuUtilization() {   
 
  std::ifstream file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  string skip, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::getline(file, line);
  std::istringstream cpudata(line);

  cpudata >> skip >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

  float usertime = stof(user) - stof(guest);
  float nicetime = stof(nice) - stof(guest);
  float idletime = stof(idle) + stof(iowait);
  float systemtime = stof(system) + stof(irq) + stof(softirq);
  float virtualtime = stof(guest) + stof(guest_nice);
  float totaltime = usertime + nicetime + idletime + systemtime + stof(steal) + virtualtime;

  return (totaltime - idletime)/totaltime;

}

float LinuxParser::ProcessCpuUtilization() { 

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

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return stof(ParseFile(kProcDirectory + kStatFilename, "processes"));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return stof(ParseFile(kProcDirectory + kStatFilename, "procs_running"));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  
  string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename;
  return LinuxParser::ParseFile(filename, 1);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 

  string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename;
  float RamKB = stof(LinuxParser::ParseFile(filename, "VmSize:"));
  return to_string(RamKB/1000).substr(0, 7); 

}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 

  string filename = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename;
  string uid = LinuxParser::ParseFile(filename, "Uid:");
    
  return uid;

}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 

  string uid = LinuxParser::Uid(pid);

  std::ifstream file(kPasswordPath);
  string line;
  string user, skip, id;

  while (std::getline(file, line)){

    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream stream(line);
    
    stream >> user >> skip >> id;
    if (id == uid){
      return user;
    }

  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 

  float systemuptime = LinuxParser::UpTime(); // in seconds
  return systemuptime - LinuxParser::ProcessStartTime(pid);

 }