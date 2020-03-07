#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <math.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    return LinuxParser::SystemCpuUtilization();
    
}