#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "playersmanager.h"

class GameState{
public:
    uint16_t m_CurrentTurnNb = 0;
    std::unordered_map<QString, int> m_DiedEnnemies; // key name, value turn number
    std::vector<QString> m_OrderToPlay;
    uint16_t m_CurrentRound = 0; // max value = size of m_OrderToPlay

    QString GetCurrentPlayerName();
};

class GameManager
{
public:
    GameManager() = default;
    PlayersManager* m_PlayersManager = nullptr;
    GameState* m_GameState;

    void InitPlayers();
    Character* GetSelectedHero();
    void ProcessOrderToPlay(std::vector<QString>& orderToPlay);
    Character* GetCurrentPlayer();
};

#endif // GAMEMANAGER_H
