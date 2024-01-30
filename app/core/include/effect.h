#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>

#include "common.h"
#include <unordered_map>

enum class EffectCategory{
    equipment = 0,
    specialAtk,
    talentPower,
    enumSize
};

enum class EffectType{
    effectOn = 0,
    nbCoolingTurns,
    nbTurnsOn,
    probaDegressOnTour,
    probaDegressNextTour,
    target,
    reach,
    valueChange,
    perCentChange,
    enumSize
};

struct effectParam{
    QString effect;
    int value = 0;
};

template <class T> class EffectStat {
public:
    explicit EffectStat(EffectType type, QString name, T value) : m_Type(type),m_Name(name), m_Value(value)  {}
    T m_Value;
    EffectType m_Type;
    QString m_Name;
};

struct EffectOnStat{
    EffectStat<bool> m_effectOn = EffectStat<bool>(EffectType::effectOn, "Effet On", false);
    EffectStat<uint8_t> m_NbCoolingTurns = EffectStat<uint8_t>(EffectType::nbCoolingTurns, "Surchauffe", false);
    EffectStat<uint8_t> m_NbTurnsOn = EffectStat<uint8_t>(EffectType::nbTurnsOn, "Nombre de tours ON", false);
    EffectStat<uint8_t> m_ProbaDegressOnTour = EffectStat<uint8_t>(EffectType::probaDegressOnTour, "% degressif ce tour", false);
    EffectStat<uint8_t> m_ProbaDegressNextTour = EffectStat<uint8_t>(EffectType::probaDegressNextTour, "% degressif au prochain tour", false);
    EffectStat<uint8_t> m_Target = EffectStat<uint8_t>(EffectType::target, "Cible", false);
    EffectStat<bool> m_Reach = EffectStat<bool>(EffectType::reach, "Portée", false);
    EffectStat<int16_t> m_ValueChange = EffectStat<int16_t>(EffectType::perCentChange, "Changement par nombre", false);
    EffectStat<int16_t> m_PerCentChange = EffectStat<int16_t>(EffectType::perCentChange, "Changement par %", false);
};

class Effect : public QObject
{
    Q_OBJECT
public:
    explicit Effect(QObject *parent = nullptr);
    std::unordered_map<QString, EffectOnStat> m_EffectOnStats;
    EffectCategory m_Category = EffectCategory::equipment;
};

#endif // EFFECT_H
