#include <string>

#include "format.h"

using std::string;

// Converts long int to HH:MM:SS format
string Format::ElapsedTime(long seconds) {
    int h, m, s;
    string hh, mm, ss;
    h = seconds / 3600;
    m = seconds % 3600 / 60;
    s = seconds % 60;
    hh = h > 9? std::to_string(h) : "0" + std::to_string(h);
    mm = m > 9? std::to_string(m) : "0" + std::to_string(m);
    ss = s > 9? std::to_string(s) : "0" + std::to_string(s);
    return hh + ":" + mm + ":" + ss; 
}