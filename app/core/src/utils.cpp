#include "utils.h"

#include <ctime>


int64_t Utils::GetRandomNb(const int64_t min, const int64_t max ){
    srand((int)time(0));
   return (rand() % (max - min)) + 1;
}
