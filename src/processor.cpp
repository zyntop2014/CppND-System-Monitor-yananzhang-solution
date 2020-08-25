#include "processor.h"
#include "linux_parser.h"
#include <unistd.h> 


using std::stof;
using std::stoi;
using std::string;
using std::to_string;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
	long total_start = LinuxParser::Jiffies();
	long active_start = LinuxParser::ActiveJiffies();
	usleep(100000); // in microseconds
	long total_end = LinuxParser::Jiffies();
	long active_end = LinuxParser::ActiveJiffies();
	return (float)(active_end - active_start) / (float)(total_end - total_start);
    
}