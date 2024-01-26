#include "gamemanager.h"

void GameManager::InitPlayers() {
  // init the game
  m_GameState = new GameState();
  // init the players
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

Character *GameManager::GetSelectedHero() {
  if (m_PlayersManager == nullptr ||
      m_PlayersManager->m_SelectedHero == nullptr) {
    return nullptr;
  }

  return m_PlayersManager->m_SelectedHero;
}

void GameManager::ProcessOrderToPlay(std::vector<QString> &orderToPlay) {
  // to be improved with stats
  // one player can play several times as well in different order
  // random ?
  orderToPlay.clear();

  for (const auto& hero : m_PlayersManager->m_HeroesList) {
    orderToPlay.push_back(hero->m_Name);
  }
  for (const auto& boss : m_PlayersManager->m_BossesList) {
    orderToPlay.push_back(boss->m_Name);
  }
}

Character* GameManager::GetCurrentPlayer(){
    const auto& nameChara = m_GameState->GetCurrentPlayerName();
    return m_PlayersManager->GetCharacterByName(nameChara);
}

/////////////////////////////////////////
/// \brief GameState::GetCurrentPlayerName
///
QString GameState::GetCurrentPlayerName(){
    return (m_CurrentRound > 0) ? m_OrderToPlay.at(m_CurrentRound-1) : "";
}

