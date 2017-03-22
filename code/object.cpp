#include "../includes/object.hpp"

using namespace std;

unsigned int gameObject::next_id=1; //Initializing game object id

gameObject::gameObject()
{
    if (gameObject::next_id != UINT_MAX) {
        id=gameObject::next_id;
        ++gameObject::next_id;
    }
}

gameObject::~gameObject()
{
    id=0;
    next_id=0;
}
