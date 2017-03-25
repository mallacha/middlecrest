#include "../includes/utility.hpp"

using namespace std;

int validateUnsignedShort(long value)
{
    if(value > USHRT_MAX) {
        return USHRT_MAX;
    }else if(value < 0){
        return 0;
    }

    return value;
}

unsigned int validateUnsignedInt(long value)
{
    if(value < 0){
        return 0;
    }

    return value;
}

/*void sleep(float duration_in_seconds)
{
    long pause = (long)(CLOCKS_PER_SEC * duration_in_seconds);
    clock_t start;
    clock_t now;

    start = clock();
    now=start;
    while((now-start) < pause) {
        now=clock();
    }
}*/


        #ifdef UNIT_TEST
            clock_t start=0;
            clock_t stop=0;
            int duration=0;

            void timerSet(int duration_in_seconds)
            //TODO: perhaps replaced by a more useful one
            {
                start=clock();
                stop = start + (CLOCKS_PER_SEC * duration_in_seconds);
            }

            int timerFinished()
            {
                if(clock() > stop) {
                    return 1;
                }

                return 0;
            }
        #endif

