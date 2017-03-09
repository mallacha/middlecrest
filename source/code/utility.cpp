#include "../includes/utility.hpp"
#include <string.h>

using namespace std;

long int stringToInt(char * buffer)
{
    char * pEnd;

    return strtol(buffer, &pEnd, 10);
}

double stingToDecimal(char * buffer)
{
    char * pEnd;

    strtod(buffer, &pEnd);
}

void junkTheArray(char *& array)
{
    srand(time(0));

    memset(array, rand(), strlen(array));

    array[0] = '\0';
}
