#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include "effect.h"

class Character;
class AttaqueType;
class GameAtkEffects;

class Utils {
public:
  Utils() = default;
  static bool CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2);
  static bool CompareBySpeed(const Character *c1, const Character *c2);
  static QString getCurrentTimeAsString();
  static std::optional<AllEffectsType>
  GetActiveEffects(const std::vector<GameAtkEffects> &table);
  static int64_t GetRandomNb(const int64_t min, const int64_t max);
  static bool FileExists(const QString &path);
  static bool RemoveFile(const QString &path);
  static double CalcRatio(const int val1, const int val2);
  static std::pair<QJsonObject, QString> LoadJsonFile(const QString& filepath);
};

#endif // UTILS_H
