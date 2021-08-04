#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    int minutes,mins, secs,hours;
    string hrs,min, sec;

    minutes= seconds/60;
    hours=minutes/60;
    mins=minutes%60;
    secs=seconds%60;

    hrs = hours<10 ? "0" + std::to_string(hours) : std::to_string(hours);
    min = mins<10 ? "0" + std::to_string(mins) : std::to_string(mins);
    sec = secs<10 ? "0" + std::to_string(secs) : std::to_string(secs);
    
    return hrs+":"+min+":"+sec; 

}