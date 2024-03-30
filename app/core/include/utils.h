#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include "character.h"

class Utils
{
public:
    Utils() = default;
    static int64_t GetRandomNb(const int64_t min, const int64_t max);
    static bool CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2);
};

#endif // UTILS_H
