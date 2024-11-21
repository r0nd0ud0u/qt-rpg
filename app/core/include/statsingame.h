#ifndef STATSENDGAME_H
#define STATSENDGAME_H

#include <QObject>
#include "unordered_map"

struct AtksInfo{
    QString atkName;
    int nbOfUse = 0;
    std::unordered_map<QString, int> allDamagesByTarget; // key target
};

// Forward declaration
class Character;

class StatsInGame
{
public:
    bool m_IsPlaying = false;
    bool m_IsAlive = true;
    std::unordered_map<QString, AtksInfo> m_AllAtksInfo;

    void StartGame(const bool isSelected);

    static void GenerateStatsEndGame(const QString& filepath);
    static QString GetOutputEndGameByList(const std::vector<Character *> &playerList);
};

#endif // STATSENDGAME_H
