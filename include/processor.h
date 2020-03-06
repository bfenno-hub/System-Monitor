#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
using std::string;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void UpdateValues();
  void Initialize(); 
  Processor(); 
  // TODO: Declare any necessary private members
 private:
  
  std::string user;
  std::string nice;
  std::string system;
  std::string idle;
  std::string iowait; 
  std::string irq; 
  std::string softirq; 
  std::string steal; 
  std::string guest; 
  std::string guest_nice;

  float usertime{0};
  float nicetime{0}; 
  float idletime{0}; 
  float systemtime{0}; 
  float virtualtime{0}; 
  float totaltime{0};
};

#endif