#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "playersmanager.h"

#include <memory>

class GameManager
{
public:
    GameManager() = default;
    std::unique_ptr<PlayersManager> m_PlayersManager = nullptr;
    void InitPlayers();
private:
    void InitHeroes();
    void InitBosses();
};

#endif // GAMEMANAGER_H
