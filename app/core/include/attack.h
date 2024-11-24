#ifndef ATTACK_H
#define ATTACK_H

#include <QObject>

#include <vector>

#include "common.h"
#include "rust-rpg-bridge/attaque.h"
#include "effect.h"

class AttaqueType {
public:
    QString name = "Atq";
    uint8_t level = 1;
    uint32_t manaCost = 0;
    uint32_t vigorCost = 0;
    uint32_t berseckCost = 0;
    QString target = TARGET_ENNEMY;
    QString reach = REACH_INDIVIDUAL;
    QString namePhoto = "default.png";
    std::vector<effectParam> m_AllEffects = {};
    QString form = STANDARD_FORM;
    AttaqueNature nature;
    QString atkSoundPath;

    static constexpr double COEFF_CRIT_STATS = 1.5;
    static constexpr double COEFF_CRIT_DMG = 2.0;
};


class Attack
{
public:
    Attack() = default;
};

struct EffectOutcome{
    QString logDisplay;
    std::vector<effectParam> newEffects;
    int fullAtkAmountTx;
    int realAmountTx;
    QString targetName;
    AttaqueType atk;
    bool isCritical;
};

#endif // ATTACK_H
