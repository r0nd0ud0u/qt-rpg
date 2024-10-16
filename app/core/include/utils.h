#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Character;
class AttaqueType;
class GameAtkEffects;

struct EffectsTypeNb {
  int buf = 0;
  int debuf = 0;
  int hot = 0;
  int dot = 0;
  int oneTurnEffect = 0;
};

class Utils {
public:
  Utils() = default;
  static bool CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2);
  static bool CompareBySpeed(const Character *c1, const Character *c2);
  static QString getCurrentTimeAsString();
  static std::optional<EffectsTypeNb>
  GetNbOfActiveEffects(const std::vector<GameAtkEffects> &table);
  static int64_t GetRandomNb(const int64_t min, const int64_t max );
  static bool FileExists(const QString& path);
  static bool RemoveFile(const QString& path);
};

#endif // UTILS_H
