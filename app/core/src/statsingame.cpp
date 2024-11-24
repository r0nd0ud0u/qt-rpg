#include "statsingame.h"

#include "Application.h"
#include "character.h"

#include <QDir>
#include <QFile>

void StatsInGame::GenerateStatsEndGame(const QString &filepath) {
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
  const QString logFilePath =
      logDir.filePath(QString(filepath).arg(Utils::getCurrentTimeAsString()));
  file.setFileName(logFilePath);
  logDir.mkpath(QFileInfo(filepath).absolutePath());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Application::GetInstance().log(" Could not open the file for writing " +
                                   logFilePath);
  }
  QTextStream out(&file);
  // title
  out << ENDGAME_TITLE_BAR.join(";\t") << "\n";
  out << GetOutputEndGameByList(pm->m_HeroesList).toUtf8() << "\n";
  out << GetOutputEndGameByList(pm->m_BossesList).toUtf8() << "\n";
  file.close();
}

QString StatsInGame::GetOutputEndGameByList(
    const std::vector<Character *> &playerList) {
  if (playerList.empty()) {
    return "";
  }

  QString output;
  // 5 columns
  const QString values = QStringList({"%1", "%2", "%3", "", ""}).join(";\t");
  for (const auto *h : playerList) {
    if (h == nullptr || !h->m_StatsInGame.m_IsPlaying) {
      continue;
    }
    const auto charaType = (h->m_Type == characType::Hero) ? "Hero" : "Boss";
    const auto lifeStatus = h->m_StatsInGame.m_IsAlive ? "Alive" : "Dead";
    // update output with header
    output += values.arg(h->m_Name, charaType, lifeStatus) + "\n";
    // update output attacks
    output += ";Attaks;Attaks used;Attaks frequency;Dmgs on target\n";
    for (const auto &[atkName, atkType] : h->m_AttakList) {
      int nbOfUse = 0;
      QString damageByTarget;
      if (h->m_StatsInGame.m_AllAtksInfo.count(atkName) > 0) {
        nbOfUse = h->m_StatsInGame.m_AllAtksInfo.at(atkName).nbOfUse;
        for (const auto &[name, dmg] :
             h->m_StatsInGame.m_AllAtksInfo.at(atkName).allDamagesByTarget) {
          damageByTarget += QString(";;;;%1:%2\n").arg(name).arg(dmg);
        }
      }
      output += QString(";;%1;%2\n").arg(atkName).arg(nbOfUse);
      output += damageByTarget;
    }
    std::unordered_map<QString, int> allDmgByTarget;
    for (const auto &[atkName, info] : h->m_StatsInGame.m_AllAtksInfo) {
      for (const auto &[target, dmg] : info.allDamagesByTarget)
        allDmgByTarget[target] += dmg;
    }
    output += ";Stats on all targets\n";
    for (const auto &[target, dmg] : allDmgByTarget) {
      output += QString(";;%1;%2\n").arg(target).arg(dmg);
    }
    // update output with stats by turn RX TX
    output += ";Stats RX TX\n";
    for (int i = 0; i < h->m_LastTxRx.size(); i++) {
      const auto &amountType = amountTypeStr[i];
      uint64_t value = 0;
      for (const auto &[turnNb, turnValue] : h->m_LastTxRx.at(i)) {
        value += turnValue;
      }
      output += QString(";;%1;%2").arg(amountType).arg(value) + "\n";
    }
  }

  return output;
}

void StatsInGame::StartGame(const bool isSelected) {
  m_IsPlaying = isSelected;
  m_IsAlive = true;
  m_AllAtksInfo.clear();
}
