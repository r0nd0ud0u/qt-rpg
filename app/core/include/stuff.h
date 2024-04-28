#ifndef STUFF_H
#define STUFF_H

#include "common.h"

#include <QString>

class Stuff
{
public:
    Stuff() = default;

    Stats m_Stats = {};
    QString m_UniqueName;
    QString m_Name;
    QString m_BodyPart;
    int m_Rank = 0;
    std::vector<QString> m_StatsUpByLoot;
};

struct EditStuff {
    Stuff m_Stuff;
    QString m_Name;
    QString m_BodyPart;
    bool updated = false;
    static void SaveStuffInJson(const EditStuff &es, const QString& bodyPart);
};

#endif // STUFF_H
