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

    if(hours<10){
        hrs="0"+std::to_string(hours);
    }else{
        hrs=std::to_string(hours);
    }
    if(mins<10){
        min="0"+std::to_string(mins);
    }else{
        min=std::to_string(mins);
    }
    if(secs<10){
        sec="0"+std::to_string(secs);
    }else{
        sec=std::to_string(secs);
    }
    return hrs+":"+min+":"+sec; 

}