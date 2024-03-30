#include "utils.h"

#include <ctime>

/**
 * @brief Utils::GetRandomNb
 * Returns an integer between a min and a max
 * Note : init the seed to get a random value each time the software is started
 */
int64_t Utils::GetRandomNb(const int64_t min, const int64_t max) {
  return (rand() % (max - min)) + 1;
}

// Define a comparison function for sorting based on the 'a' member
bool Utils::CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2) {
  return obj1.level < obj2.level;
}
