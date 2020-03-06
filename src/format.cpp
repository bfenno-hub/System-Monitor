#include <iostream>
#include <math.h>
#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int tseconds) { 
   
   string mstring, hstring, sstring;
   
   long int minutes = (tseconds/60) % 60;
   mstring = (minutes < 10) ? "0" + to_string(minutes) : to_string(minutes);
   long int hours = tseconds/3600;
   hstring = (hours < 10) ? "0" + to_string(hours) : to_string(hours);
   long int seconds = tseconds % 60;
   sstring = (seconds < 10) ? "0" + to_string(seconds) : to_string(seconds);

   return hstring + ":" + mstring + ":" + sstring;
}