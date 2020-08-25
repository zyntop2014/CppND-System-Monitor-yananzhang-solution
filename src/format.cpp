#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hour = seconds / 3600;
    int rest_seconds = seconds % 3600;
    int minutes = rest_seconds / 60;
    int secs  = rest_seconds % 60;

    return to_string(hour) + ":" + to_string(minutes) + ":" + to_string(secs); 
}