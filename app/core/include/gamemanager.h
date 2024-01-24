#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "playersmanager.h"

#include <memory>


class GameState{
public:
    uint16_t m_CurrentTurnNb = 0;
    std::unordered_map<QString, int> m_DiedEnnemies; // key name, value turn number
};

class GameManager
{
public:
    GameManager() = default;
    PlayersManager* m_PlayersManager = nullptr;
    void InitPlayers();

};

#endif // GAMEMANAGER_H
