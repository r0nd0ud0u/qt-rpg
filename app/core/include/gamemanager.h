#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "playersmanager.h"

#include <memory>

class GameState{
public:
    uint16_t m_CurrentTurnNb = 0;
    std::unordered_map<QString, int> m_DiedEnnemies; // key name, value turn number
    std::vector<QString> m_OrderToPlay;
    uint16_t m_CurrentRound = 0; // max value = size of m_OrderToPlay
};

class GameManager
{
public:
    GameManager() = default;
    PlayersManager* m_PlayersManager = nullptr;
    std::unique_ptr<GameState> m_GameState;

    void InitPlayers();
    Character* GetSelectedHero();
    void ProcessOrderToPlay(std::vector<QString>& orderToPlay)const;
};

#endif // GAMEMANAGER_H
