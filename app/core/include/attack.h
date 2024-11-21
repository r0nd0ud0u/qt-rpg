#ifndef ATTACK_H
#define ATTACK_H

#include <QObject>

#include <vector>

// forward declaration
class effectParam;

class Attack
{
public:
    Attack();
};

struct EffectOutcome{
    QString logDisplay;
    std::vector<effectParam> newEffects;
    int fullAtkAmountTx;
    int realAmountTx;
};

#endif // ATTACK_H
