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

/**
 * @brief Utils::BuildEffectName
 * Returns the concatenation of effect str and stats str
 * If the effect str name is empty => only the stats str
 * If the stats str name is empty => only the effect str
 */
QString Utils::BuildEffectName(const QString &rawEffect,
                               const QString &statsName) {
  QString effectName;
  if (rawEffect.isEmpty() && !statsName.isEmpty()) {
    effectName = statsName;
  } else if (!rawEffect.isEmpty() && statsName.isEmpty()) {
      effectName = rawEffect;
  }else if(!rawEffect.isEmpty() && !statsName.isEmpty()){
    effectName = statsName + "-" + rawEffect;
  }
  return effectName;
}

// Define a comparison function for sorting based on the 'a' member
bool Utils::CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2) {
  return obj1.level < obj2.level;
}
