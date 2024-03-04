#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils
{
public:
    Utils() = default;
    static int64_t GetRandomNb(const int64_t min, const int64_t max);
    static QString BuildEffectName(const QString& rawEffect, const QString& statsName);

};

#endif // UTILS_H
