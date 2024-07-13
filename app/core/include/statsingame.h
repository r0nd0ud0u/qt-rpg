#ifndef STATSENDGAME_H
#define STATSENDGAME_H

#include "qobject.h"

struct AtksInfo{
    QString atkName;
    int nbOfUse = 0;
};

// Forward declaration
class Character;

class StatsInGame
{
public:
    bool m_IsPlaying = false;
    bool m_IsAlive = true;
    std::vector<AtksInfo> m_AllAtksInfo;

    void StartGame(const bool isSelected);

    static void GenerateStatsEndGame();
    static QString GetOutputEndGameByList(const std::vector<Character *> &playerList);
};

#endif // STATSENDGAME_H
