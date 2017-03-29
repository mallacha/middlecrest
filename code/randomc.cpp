#include "../includes/service.hpp"

using namespace std;

IRand::~IRand()
{
}

simpleRandom::simpleRandom()
{
    seed = time(0);
    srand(seed);
    next=seed;
}

simpleRandom::~simpleRandom()
{
    seed=0;
    next=0;
}

long simpleRandom::randomInit(unsigned int value) {
// re-seed generator
    seed = value;
    next = seed;
    return seed;
}

unsigned int simpleRandom::intRandom(long min, long max)
{
    return (long)(random()%(max-min)+min);
}

unsigned int simpleRandom::random()
{
    next = next * 1103515245 + 12345;

        #if defined(ASSERTION_TEST) || defined(UNIT_TEST)
                assert((next % 32768) == (next & 32767));
        #endif

    return (unsigned int)((next/65536) & 32767);
}

void nullRNG::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullRandom object.");
}
