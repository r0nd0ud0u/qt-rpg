#include "gamemanager.h"

#include "utils.h"

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
  m_PlayersManager->InitRandomEquip();
  m_PlayersManager->LoadAllCharactersJson();
  m_PlayersManager->InitHeroes();
  m_PlayersManager->InitBosses();
}

Character *GameManager::GetSelectedHero() {
  if (m_PlayersManager == nullptr ||
      m_PlayersManager->m_SelectedHero == nullptr) {
    return nullptr;
  }

  return m_PlayersManager->m_SelectedHero;
}

/**
 * @brief GameManager::ProcessOrderToPlay
 *
 */
void GameManager::ProcessOrderToPlay(std::vector<QString> &orderToPlay) const {
  // to be improved with stats
  // one player can play several times as well in different order
  orderToPlay.clear();

  // sort by speed
  std::sort(m_PlayersManager->m_HeroesList.begin(), m_PlayersManager->m_HeroesList.end(), Utils::CompareBySpeed);
  std::vector<QString> deadHeroes;
  for (const auto &hero : m_PlayersManager->m_HeroesList) {
    if (hero->IsDead()) {
      orderToPlay.push_back(hero->m_Name);
    } else {
        deadHeroes.push_back(hero->m_Name);
    }
  }

  std::for_each(deadHeroes.begin(), deadHeroes.end(), [&](const QString& name){
      orderToPlay.push_back(name);
  });
  for (const auto &boss : m_PlayersManager->m_BossesList) {
    orderToPlay.push_back(boss->m_Name);
  }
  // supplementariy atks to push
  m_PlayersManager->AddSupAtkTurn(characType::Hero, orderToPlay);
  m_PlayersManager->AddSupAtkTurn(characType::Boss, orderToPlay);
}

QString GameManager::ProcessLogOrderToPlay() const {
  QString logs;
  if (m_GameState == nullptr) {
    return "";
  }
  if (!m_GameState->m_OrderToPlay.empty()) {
    logs.append("Ordre des joueurs:\n");
  }
  int counter = 1;
  std::for_each(m_GameState->m_OrderToPlay.begin(),
                m_GameState->m_OrderToPlay.end(),
                [&logs, &counter](const QString &plName) {
                  logs += QString("%1 %2\n").arg(counter).arg(plName);
                  counter++;
                });

  return logs;
}

Character *GameManager::GetCurrentPlayer() {
  if (m_GameState == nullptr) {
    return nullptr;
  }
  const auto nameChara = m_GameState->GetCurrentPlayerName();
  return m_PlayersManager->GetCharacterByName(nameChara);
}

/////////////////////////////////////////
/// \brief GameState::GetCurrentPlayerName
///
QString GameState::GetCurrentPlayerName() {
  return (m_CurrentRound > 0) ? m_OrderToPlay.at(m_CurrentRound - 1) : "";
}

void GameState::RemoveDeadPlayerInTurn(const QString &name) {
  const auto newEnd = std::remove_if(
      m_OrderToPlay.begin(), m_OrderToPlay.end(),
      [&name](const QString &localName) {
        return localName == name; // remove elements where this is true
      });
  m_OrderToPlay.erase(newEnd, m_OrderToPlay.end());
}
