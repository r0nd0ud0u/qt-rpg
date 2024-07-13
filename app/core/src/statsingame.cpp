#include "statsingame.h"

#include "Application.h"
#include "character.h"

#include <QDir>
#include <QFile>

void StatsInGame::GenerateStatsEndGame() {
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
  const auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm == nullptr) {
    return;
  }
  const auto *pm = gm->m_PlayersManager;
  if (pm == nullptr) {
    return;
  }

  out << GetOutputEndGameByList(gm->m_PlayersManager->m_AllHeroesList) << "\n";
  out << GetOutputEndGameByList(gm->m_PlayersManager->m_AllBossesList) << "\n";
}

QString StatsInGame::GetOutputEndGameByList(const std::vector<Character *> &playerList) {
    if (playerList.empty()) {
        return "";
    }

    QString output;

    QString values = QStringList({"%1", "%2", "%3", "%4", "%5"}).join(";\t");
    for (const auto *h : playerList) {
        if (h != nullptr && h->m_StatsInGame.m_IsPlaying) {
            const auto type = (h->m_type == characType::Hero) ? "Hero" : "Boss";
            const auto lifeStatus = h->m_StatsInGame.m_IsAlive ? "Alive" : "Dead";
            const auto atksUsed =
                !h->m_StatsInGame.m_AllAtksInfo.empty() ? "OK" : "None";
            output += values.arg(h->m_Name, type, lifeStatus, atksUsed, atksUsed) + "\n";
        }
    }

    return output;
}

void StatsInGame::StartGame(const bool isSelected) {
  m_IsPlaying = isSelected;
  m_IsAlive = true;
  m_AllAtksInfo.clear();
}
