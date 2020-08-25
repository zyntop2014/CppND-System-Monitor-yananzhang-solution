#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <iomanip>

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

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
  string os, kernel, version;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float totalMemo, freeMemo;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
      while(std::getline(filestream, line)) {
        std:: istringstream linestream(line);
        linestream >> key >> value;
        if(key == "MemTotal:") {
          totalMemo = stoi(value);
        }
        if(key == "MemFree:") {
          freeMemo = stoi(value);
        }
      }
  }
  return (totalMemo - freeMemo) / totalMemo;
 }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std:: istringstream linestream(line);
      linestream >> key >> value;
      upTime = stol(key);
  }
  return upTime; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int state = kUser_; state  <= kSteal_; state++){
    total = total + stol(cpuUtilization[state]);
  }
  return total;
 }

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, lineInfo;
  long total {0};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (std::getline(linestream, lineInfo, ' ')) {
      if ((i == 13) || (i == 14) || (i == 15) || (i == 16)) {
        total += stol(lineInfo);
      }
      i++; 
    }
  }
  return total / sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return  LinuxParser::Jiffies() - LinuxParser::IdleJiffies();; }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long total = 0;
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  for (int state = kIdle_; state <= kIOwait_; state++){
    total += stol(cpuUtilization[state]);
  }
  return total;
 }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::vector<string> cpuUtilization;
  string line, key, linInfo;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std:: istringstream linestream(line);
      while(std::getline(linestream, linInfo, ' ')) {
        if(linInfo != "cpu" && linInfo != "") {
          cpuUtilization.push_back(linInfo);
        }
      }
  }
  return cpuUtilization; 
 }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      while(std::getline(filestream, line)) {
        std:: istringstream linestream(line);
        linestream >> key >> value;
        if(key == "processes") {
          total = stoi(value);
          break;
        }
      }
  }
  return total; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      while(std::getline(filestream, line)) {
        std:: istringstream linestream(line);
        linestream >> key >> value;
        if(key == "procs_running") {
          running = stoi(value);
          break;
        }
      }
  }
  return running; 
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, res;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    return line;
  }
  return "NONE";
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:"){
          float mem = stof(value) / 1000;
          std::stringstream ss;
          ss << std::fixed << std::setprecision(3) << mem;
          return ss.str();
        }
      }
    }
  }
  return "0";
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:"){
          return value;
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {  
  string line, x, user,fileUid ;
  string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> fileUid;
      if(uid == fileUid) {
        return user;
      }
    }
  }
  return "None"; 
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, time;
  int i{0};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (std::getline(linestream, time, ' ')) {
      if (i == 21) {
        return (stol(time) / sysconf(_SC_CLK_TCK)) ;
      }
      i++;
    }
  }
return 0;
}