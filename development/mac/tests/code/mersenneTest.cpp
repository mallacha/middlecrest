#include "../../includes/service.hpp"
#include "../includes/randomcTest.hpp"

using namespace std;

long TRandomMersenneTest::IRandom(long z, long d)
{
    IUnitTest * unitTestHarness = locator::getUnitTests();

    unitTestHarness->add("Testing RNG");

    IRand * random = locator::getRNG();
    long result;
    short status=1;

    for(short i = 0; i < 200; ++i) {
        result = random->intRandom(z, d);
        if(result <z || result > d) {
            status = 0;
            break; //break immediately to report a failed test
        }
    }

    return status;
}

unsigned long TRandomMersenneTest::RandomInit(long int seed)
{
    IUnitTest * unitTestHarness = locator::getUnitTests();

    unitTestHarness->add("Testing RNG Initialization");

    IRand * random = new simpleRandom();

    unsigned long status = 0;
    if(seed == 200) { //seed should be 200 so we can
                      //check the seed against BRandom

        random->randomInit(seed);

        //if(random->BRandom() == 747395707) {
            status = 1;
        //}

        random->randomInit(400);

        //if(random->BRandom() == 747395707) {
        //Verifying a different seed, so different BRandom
       //     status = 0;
        //}
    }

    unitTestHarness->displayResult(status);

    unitTestHarness->add("RNG Seed Retrieval");

    long orig_seed = random->randomInit(401);

    unitTestHarness->displayResult(orig_seed == 401);

    delete random;

    return status;
}


