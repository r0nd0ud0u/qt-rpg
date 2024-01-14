#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "heroes.h"

#include <memory>

class GameManager
{
public:
    GameManager() = default;
    std::unique_ptr<Heroes> m_Heroes = nullptr;

    void InitHeroes();
};

#endif // GAMEMANAGER_H
