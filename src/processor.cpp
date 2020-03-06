#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <math.h>

Processor::Processor(){
    Initialize();
}

void Processor::Initialize(){
    UpdateValues();
}

void Processor::UpdateValues(){

    std::ifstream file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    
    if (file.is_open()){

        string skip;
        string line;
        std::getline(file, line);
        std::istringstream cpudata(line);

        cpudata >> skip >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        usertime = stof(user) - stof(guest);
        nicetime = stof(nice) - stof(guest);
        idletime = stof(idle) + stof(iowait);
        systemtime = stof(system) + stof(irq) + stof(softirq);
        virtualtime = stof(guest) + stof(guest_nice);
        totaltime = usertime + nicetime + idletime + systemtime + stof(steal) + virtualtime;
    }

    file.close();

}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    //simple version
    UpdateValues();
    //std::cout << (totaltime - idletime)/totaltime << "\n";
    return (totaltime - idletime)/totaltime;
    
    /*
    //more accurate version
    //returning util causes *** %n in writable segment detected *** error (wprintw?)
    float prevtotal = totaltime;
    float previdle = stof(idle);
    UpdateValues();
    float totald = totaltime - prevtotal;
    float idled = stof(idle) - previdle;
    float util = (totald - idled)/totald;
    //std::cout << util << "\n";
    return util;
    */
}