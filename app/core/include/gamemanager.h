#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "playersmanager.h"

#include <memory>

class GameManager
{
public:
    GameManager() = default;
    PlayersManager* m_PlayersManager = nullptr;
    void InitPlayers();

};

#endif // GAMEMANAGER_H
