#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// Based off of the solution at:
// https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key;
  float memTotal, memFree;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> memTotal;
  }
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> memFree;
  }
  return (memTotal - memFree) / memTotal;
  }

// Reads and returns the system uptime
long int LinuxParser::UpTime() {
  long int uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

// Based off of solution posted at:
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
// Read and return the cpu usage for a PID
long LinuxParser::CpuUsage(int pid) { 
  string line;
  string value;
  float totalTime, uTime, sTime, cuTime, csTime, startTime, elapsedTime, usage;
  long int hertz = sysconf(_SC_CLK_TCK);
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=1;i<23;i++) {
      linestream >> value;
      if(i == 14)
        uTime = stol(value);
      if(i == 15)
        sTime = stol(value);
      if(i == 16)
        cuTime = stol(value);
      if(i == 17)
        csTime = stol(value);
      if(i == 21)
        startTime = stol(value);
    }
    totalTime = uTime + sTime + cuTime + csTime;
    elapsedTime = UpTime() - (startTime / hertz);
    usage = (totalTime / hertz ) / elapsedTime;
    return usage;
  }
  return long();
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string cpuId, user, nice, system, idle, iowait, irq, softirq; 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpuId >> user >> nice >> system >> idle >> iowait >> irq >> softirq;
  }
  return vector<string> {user, nice, system, idle, iowait, irq, softirq};
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { return sizeof(Pids()); }

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return int();
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "VmSize:") { 
          return to_string(stoi(value) / 1024); 
        }
    }
  }
  return string();
}

// Reads and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "Uid:") { return value; }
    }
  }
  return string();
}

// Reads and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string key, passwd;
  int value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> passwd >> value) {
        if (value == pid) {
          return key;
        }
      }
    }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=1;i<23;i++) {
      linestream >> value;
      if(i == 22)
      {
        return LinuxParser::UpTime() - stol(value) / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return long();
}