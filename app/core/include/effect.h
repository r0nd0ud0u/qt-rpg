#ifndef EFFECT_H
#define EFFECT_H

#include <QObject>

// #include "common.h"
// #include <unordered_map>

struct effectTypes{
    uint8_t nbCoolingTurns = 0;
};

class Effect : public QObject
{
    Q_OBJECT
public:
    explicit Effect(QObject *parent = nullptr);
   // std::unordered_map<Stats, effectTypes> m_

signals:
};

#endif // EFFECT_H
