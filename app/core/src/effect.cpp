#include "effect.h"

#include "common.h"

#include <QJsonObject>

QJsonObject effectParam::EffectToJsonArray() const{
    QJsonObject item;
    item[EFFECT_TYPE] = effect;
    item[EFFECT_VALUE] = value;

    item[EFFECT_TARGET] = target;
    item[EFFECT_REACH] = reach;
    item[EFFECT_STAT] = statsName;
    item[EFFECT_ACTIVE_TURNS] = nbTurns;
    item[EFFECT_SUB_VALUE] = subValueEffect;

    item[EFFECT_PASSIVE_TALENT] = passiveTalent;
    item[EFFECT_COUNTER_TURN] = counterTurn;
    item[EFFECT_IS_MAGIC] = isMagicAtk;

    return item;
}

// effectParam effectParam::JsonArrayToEffect(QJsonArray) const{
//     QJsonObject item;
//     item[EFFECT_ARRAY] = effect;
//     item[EFFECT_VALUE] = value;

//     item[EFFECT_TARGET] = target;
//     item[EFFECT_REACH] = reach;
//     item[EFFECT_STAT] = statsName;
//     item[EFFECT_ACTIVE_TURNS] = nbTurns;
//     item[EFFECT_SUB_VALUE] = subValueEffect;

//     item[EFFECT_PASSIVE_TALENT] = passiveTalent;
//     item[EFFECT_COUNTER_TURN] = counterTurn;
//     item[EFFECT_IS_MAGIC] = isMagicAtk;

//     return item;
// }
