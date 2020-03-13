#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, string user, string cmdline, float cpu, string ram, long int uptime) :
  pid_(pid), user_(user), cmdline_(cmdline), cpu_(cpu), ram_(ram), uptime_(uptime) {}
  int Pid();                               
  string User();                     
  string Command();                  
  float CpuUtilization();                  
  string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;

 private:
  int pid_;
  string user_;
  string cmdline_;
  float cpu_;
  string ram_;
  long int uptime_;
};

#endif