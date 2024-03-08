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

QString Utils::ComputeNbOfShots(const QString &name, const int value) {
  QString output;

  std::set<int> shotSeb;
  std::set<int> shotMarc;
  std::set<int> shotKiki;

  for (int i = 41; i < 72; i++) {
    shotSeb.insert(i);
  }
  for (int i = 70; i < 101; i++) {
    shotKiki.insert(i);
  }
  for (int i = 15; i < 16; i++) {
    shotMarc.insert(i);
  }
  const bool isSebShot = name == "Azrak Ombresang" && shotSeb.count(value) > 0;
  const bool isMarcShot = name == "Thraïn" && shotMarc.count(value) > 0;
  const bool isKikiShot = name == "Thalia" && shotKiki.count(value) > 0;
  if (isSebShot || isMarcShot || isKikiShot) {
    output += " Prend ton shot!!!!\n";
  }

  return output;
}
