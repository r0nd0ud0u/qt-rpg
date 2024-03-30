#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include "character.h"

class Utils
{
public:
    Utils() = default;
    static bool CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2);
};

#endif // UTILS_H
