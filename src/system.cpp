#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Potentially used to sort Process class
bool sortProc(Process& a, Process& b) { return a < b; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    vector<int> processList = LinuxParser::Pids();
    for(auto process : processList)
    {
        Process newProcess(Process(process, LinuxParser::User(stoi(LinuxParser::Uid(process))), LinuxParser::Command(process),
        LinuxParser::CpuUsage(process), LinuxParser::Ram(process), LinuxParser::UpTime(process)));
        processes_.push_back(newProcess);
    }
    std::sort(processes_.begin(), processes_.end(), sortProc);
    return processes_; 
}

// Returns the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Returns the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Returns the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Returns the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }