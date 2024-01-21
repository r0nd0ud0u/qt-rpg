#include "gamemanager.h"

#include "character.h"

void GameManager::InitPlayers() {
  m_PlayersManager = new PlayersManager();
    m_PlayersManager->InitHeroes();
  m_PlayersManager->InitBosses();
  m_PlayersManager->m_SelectedHero = m_PlayersManager->m_HeroesList.front();
}


