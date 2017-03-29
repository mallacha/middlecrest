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

long simpleRandom::randomInit(long int value) {
// re-seed generator
    seed = value;

    return seed;
}

long simpleRandom::intRandom(long min, long max)
{
    return (long)(random()%(max-min)+min);
}

unsigned int simpleRandom::random()
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void nullRNG::log()
{
    logger = locator::getLogging();
    logger->warn("Register service, using nullRandom object.");
}
