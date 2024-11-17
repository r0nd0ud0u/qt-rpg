#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>

struct effectParam {
  // received
  QString effect;
  int value = 0;
  int nbTurns = 0;
  int subValueEffect = 0;
  QString target;
  QString reach;
  QString statsName;
  // processed
  int counterTurn = 0;
  bool isMagicAtk = false; // magical or physical effect
  bool passiveTalent = false;
  QJsonObject EffectToJsonObject() const;
};

struct EffectType {
  int nb = 0;
  QStringList effects;
};

struct AllEffectsType {
  EffectType buf;
  EffectType debuf;
  EffectType hot;
  EffectType dot;
  int oneTurnEffect = 0;
};

#endif // EFFECT_H
