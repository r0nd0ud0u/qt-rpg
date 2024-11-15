#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "playersmanager.h"

class GameState {
public:
  uint64_t m_CurrentTurnNb = 0;
  std::unordered_map<int, QString>
      m_DiedEnnemies; // key turn number, value name
  std::vector<QString> m_OrderToPlay;
  uint64_t m_CurrentRound = 0; // max value = size of m_OrderToPlay
  QString m_GameName;

  QString GetCurrentPlayerName();
  void RemoveDeadPlayerInTurn(const QString &name);
  void OutputGameStateOnJson(const QString& filepath);
};

struct GamePaths{
    QString characterPath;
    QString equipmentPath;
    QString lootEquipment;
    QString ongoingEffectsPath;
    QString gameState;
    QString statsInGame;
};

class GameManager {
public:
  GameManager() = default;
  PlayersManager *m_PlayersManager = nullptr;
  GameState *m_GameState;
  GamePaths m_Paths;

  void InitPlayers();
  Character *GetSelectedHero();
  void ProcessOrderToPlay(std::vector<QString> &orderToPlay) const;
  Character *GetCurrentPlayer();
  QString ProcessLogOrderToPlay() const;
  void SaveGame();
  void LoadGame();
  void StartGame();
  QString GetEquipmentPath(const bool isLoot)const;
};

#endif // GAMEMANAGER_H
