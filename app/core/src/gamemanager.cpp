#include "gamemanager.h"

#include "Application.h"

void GameManager::InitPlayers() {
  m_PlayersManager = new PlayersManager();
  // order is important
  // first equipment
  // then heroes or bosses
  // finally selected hero is init
  m_PlayersManager->LoadAllEquipmentsJson();
  m_PlayersManager->InitHeroes();
  m_PlayersManager->InitBosses();
  m_PlayersManager->m_SelectedHero = m_PlayersManager->m_HeroesList.front();
}

Character* GameManager::GetSelectedHero(){
    if (m_PlayersManager == nullptr ||
        m_PlayersManager->m_SelectedHero == nullptr) {
        return nullptr;
    }

    return m_PlayersManager->m_SelectedHero;
}
