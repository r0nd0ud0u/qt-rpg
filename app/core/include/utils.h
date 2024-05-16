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
  int oneTurnHotDot = 0;
};

class Utils {
public:
  Utils() = default;
  static bool CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2);
  static bool CompareBySpeed(const Character *c1, const Character *c2);
  static QString getCurrentTimeAsString();
  static std::optional<EffectsTypeNb>
  GetNbOfActiveEffects(const std::vector<GameAtkEffects> &table);
};

#endif // UTILS_H
