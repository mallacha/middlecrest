#include "../includes/turnBased.hpp"

using namespace std;

duration::duration()
{
    turnsRemaining=0;
}

void duration::decrement()
{
/*    if(duration >0) {
        --duration;
    }*/
}

void duration::set(long val)
{
/*    if(val > LONNG_MAX) {
        duration = LONG_MAX;
    }else if(val < LONG_MIN) {
        duration = LONG_MIN;
    }else{
        duration=val;
    }*/

    turnsRemaining=val;
}

duration::~duration()
{
    turnsRemaining=0;
}
