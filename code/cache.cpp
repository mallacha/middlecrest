#include "../includes/cache.hpp"
#include "../includes/utility.hpp"

using namespace std;

ICache::ICache()
{
    cacheSize=0;
}

ICache::~ICache()
{
    cacheSize=0;
}

void ICache::allocate(int size)
//Allocates cache
{
    if (0 == cacheSize) {
    //Only set size once

        #ifdef ASSERTION_TEST
        //return range
            //cacheSize = (unsigned short)validateUnsignedShort((long)size);
        #else
            //return (int)validateUnsignedShort((long)size);
        #endif
    }


        #ifdef ASSERTION_TEST
            //Validate that poolSize is positive
            if(cacheSize > 0) {
                //Pool size is positive
                if(cacheSize) {
                    //Overflow size is positive
                }else{
                    //Overflow size should be zero
                    assert(cacheSize == 0);
                }
            }
        #endif
}
