#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "heroes.h"

class GameManager
{
public:
    GameManager();
    Heroes* m_Heroes = nullptr;

    void InitHeroes();
};

#endif // GAMEMANAGER_H
