#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>

struct effectParam{
    // received
    QString effect;
    int value = 0;
    int nbTurns = 0;
    int subValueEffect = 0;
    QString target;
    QString reach;
    QString statsName;

    // processed
    bool updated = false;
    int counterTurn = 0;
    bool isMagicAtk = false; // magical or physical effect
};


class Effect : public QObject
{
    Q_OBJECT
public:
    explicit Effect(QObject *parent = nullptr);
};

#endif // EFFECT_H
