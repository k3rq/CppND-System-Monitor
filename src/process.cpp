#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// Returns the command that generated this process
string Process::Command() { return cmdline_; }

// Returns this process's memory utilization
string Process::Ram() { return ram_; }

// Returns the user (name) that generated this process
string Process::User() { return user_; }

// Returns the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Overload the "less than" comparison operator for Process objects
// Sorting by ram usage
bool Process::operator<(Process const& a) const { return stoi(ram_) > stoi(a.ram_); }