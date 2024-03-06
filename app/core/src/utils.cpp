#include "utils.h"

#include <ctime>

int64_t Utils::GetRandomNb(const int64_t min, const int64_t max) {
  return (rand() % (max - min)) + 1;
}

QString Utils::BuildEffectName(const QString &rawEffect,
                               const QString &statsName) {
  QString effectName;
  if (rawEffect.isEmpty()) {
    effectName = statsName;
  } else {
    effectName = statsName + "-" + rawEffect;
  }
  return effectName;
}

// Define a comparison function for sorting based on the 'a' member
bool Utils::CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2) {
    return obj1.level < obj2.level;
}

