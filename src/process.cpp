#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long startTime = LinuxParser::UpTime(pid_);
    usleep(100000);
    long upTime = LinuxParser::UpTime(pid_);
    long seconds = upTime - startTime;
    if(seconds == 0) {return 0;}
    long total = LinuxParser::ActiveJiffies(pid_);
    float cpuUsage = 100 * (total / float(seconds));

    return cpuUsage; 
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return LinuxParser::Ram(pid_) > LinuxParser::Ram(a.pid_) ? true : false; 
}