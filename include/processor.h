#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>

using std::vector;
using std::string;

class Processor {
 public:
  float Utilization();

  // I opted to return this data directly from the LinuxParser function
 private:
  vector<string> cpuData;
};

#endif