#include <string>
#include "processor.h"
#include "linux_parser.h"

using std::stof;

// Based off of solution presented at:
// https://stackoverflow.com/questions/3017162/how-to-get-total-cpu-usage-in-linux-using-c/3017438#3017438
// Return the aggregate CPU utilization
float Processor::Utilization() {
    cpuData = LinuxParser::CpuUtilization();
    float totalJiffies = 0;
    float workJiffies = 0;
    for(int i=0; (unsigned)i < cpuData.size(); i++)
    {
        if(i < 3)
            workJiffies += stof(cpuData[i]);
        totalJiffies += stof(cpuData[i]);
    }
    return workJiffies / totalJiffies;
}