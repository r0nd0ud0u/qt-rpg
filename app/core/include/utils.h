#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include "character.h"

class Utils
{
public:
    Utils() = default;
    static bool CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2);
    static bool CompareBySpeed(const Character *c1, const Character *c2);
    static QString getCurrentTimeAsString();
};

#endif // UTILS_H
