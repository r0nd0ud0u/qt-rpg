#include "statsingame.h"

#include "Application.h"
#include "character.h"

#include <QDir>
#include <QFile>

#include <numeric>

void StatsInGame::GenerateStatsEndGame() {
  const auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm == nullptr) {
    return;
  }
  const auto *pm = gm->m_PlayersManager;
  if (pm == nullptr) {
    return;
  }
  // output
  QFile file;
  QDir logDir;
  logDir.mkpath(OUTPUT_DIR);
  const QString logFilePath = logDir.filePath(
      QString(OUTPUT_ENDGAME).arg(Utils::getCurrentTimeAsString()));
  file.setFileName(logFilePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Application::GetInstance().log(" Could not open the file for writing " +
                                   logFilePath);
  }
  QTextStream out(&file);
  // title
  out << ENDGAME_TITLE_BAR.join(";\t") << "\n";
  out << GetOutputEndGameByList(gm->m_PlayersManager->m_AllHeroesList).toUtf8()
      << "\n";
  out << GetOutputEndGameByList(gm->m_PlayersManager->m_AllBossesList).toUtf8()
      << "\n";
}

QString StatsInGame::GetOutputEndGameByList(
    const std::vector<Character *> &playerList) {
  if (playerList.empty()) {
    return "";
  }

  QString output;
  const QString values = QStringList({"%1", "%2", "%3"}).join(";\t");
  for (const auto *h : playerList) {
    if (h != nullptr && h->m_StatsInGame.m_IsPlaying) {
      const auto type = (h->m_type == characType::Hero) ? "Hero" : "Boss";
      const auto lifeStatus = h->m_StatsInGame.m_IsAlive ? "Alive" : "Dead";
      output += values.arg(h->m_Name, type, lifeStatus) + "\n";
      output += ";Attaks\n";
      for (const auto &atk : h->m_AttakList) {
        int nbOfUse = 0;
        if (h->m_StatsInGame.m_AllAtksInfo.count(atk.first) > 0) {
          nbOfUse = h->m_StatsInGame.m_AllAtksInfo.at(atk.first).nbOfUse;
        }
        output += QString(";;%1;%2").arg(atk.first).arg(nbOfUse) + "\n";
      }
      // accumulate stats by turn RX TX
      output += ";Stats RX TX\n";
      for (int i = 0; i < h->m_LastTxRx.size(); i++) {
        const auto &type = amountTypeStr[i];
        uint64_t value = 0;
        for (const auto &it : h->m_LastTxRx.at(i)) {
          value += it.second;
        }
        output += QString(";;%1;%2").arg(type).arg(value) + "\n";
      }
    }
  }

  return output;
}

void StatsInGame::StartGame(const bool isSelected) {
  m_IsPlaying = isSelected;
  m_IsAlive = true;
  m_AllAtksInfo.clear();
}
