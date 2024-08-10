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
  out.setEncoding(QStringConverter::Encoding::Utf8);
  out << GetOutputEndGameByList(gm->m_PlayersManager->m_AllHeroesList).toUtf8() << "\n";
  out << GetOutputEndGameByList(gm->m_PlayersManager->m_AllBossesList).toUtf8() << "\n";
}

QString StatsInGame::GetOutputEndGameByList(const std::vector<Character *> &playerList) {
    if (playerList.empty()) {
        return "";
    }

    QString output;
    const QString values = QStringList({"%1", "%2", "%3", "%4", "%5"}).join(";\t");
    for (const auto *h : playerList) {
        if (h != nullptr && h->m_StatsInGame.m_IsPlaying) {
            const auto type = (h->m_type == characType::Hero) ? "Hero" : "Boss";
            const auto lifeStatus = h->m_StatsInGame.m_IsAlive ? "Alive" : "Dead";
            const auto atksUsed =
                !h->m_StatsInGame.m_AllAtksInfo.empty() ? "OK" : "None";
            output += values.arg(h->m_Name, type, lifeStatus, atksUsed, atksUsed) + "\n";
            for (const auto& atk : h->m_AttakList){
                int nbOfUse = 0;
                if(h->m_StatsInGame.m_AllAtksInfo.count(atk.first) > 0){
                    nbOfUse = h->m_StatsInGame.m_AllAtksInfo.at(atk.first).nbOfUse;
                }
                output += QString(";;%1;%2").arg(atk.first).arg(nbOfUse) + "\n";
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
