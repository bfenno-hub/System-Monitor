#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  float UpTime();                       // TODO: See src/process.cpp

  bool operator<(Process&  a);  // TODO: See src/process.cpp
  
  static void sort(std::vector<Process>& processes);

  Process (int p);
  // TODO: Declare any necessary private members
 private:
  int pid; 
};

#endif