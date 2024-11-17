#include "utils.h"

#include "character.h"
#include "playersmanager.h"

#include <QFileInfo>
#include <chrono>
#include <ctime> // for std::localtime
#include <iomanip>

// Define a comparison function for sorting based on the 'a' member
bool Utils::CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2) {
  return obj1.level < obj2.level;
}

bool Utils::CompareBySpeed(const Character *c1, const Character *c2) {
  if (c1 == nullptr || c2 == nullptr) {
    return false;
  }
  return c1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue >
         c2->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
}

QString Utils::getCurrentTimeAsString() {
  const auto now = std::chrono::system_clock::now();
  const auto time_t_now = std::chrono::system_clock::to_time_t(now);
  const std::tm *local_tm = std::localtime(&time_t_now);

  std::ostringstream oss;
  oss << std::put_time(local_tm, "%Y-%m-%d-%H-%M-%S");
  return QString::fromStdString(oss.str());
}

/**
 * @brief Utils::GetActiveEffects
 * Returns the number of hot, dot, buf and dot for all the effects in one attak.
 * The number of one-turn effect (damage or heal etc., change by value and %) is
 * assessed.
 */
std::optional<AllEffectsType>
Utils::GetActiveEffects(const std::vector<GameAtkEffects> &table) {
  AllEffectsType nbs;
  // for all hot/dot and buf/debuf updates
  // hot dot symbols update
  if (!table.empty()) {
    std::for_each(table.begin(), table.end(), [&](const GameAtkEffects &gae) {
      if (!gae.allAtkEffects.passiveTalent) {
        const auto effect =
            QString("%1, stats: %2 valeur:%3, ~%4 tours (par %5)")
                .arg(gae.allAtkEffects.effect)
                .arg(gae.allAtkEffects.statsName)
                .arg(gae.allAtkEffects.value)
                .arg(gae.allAtkEffects.nbTurns - gae.allAtkEffects.counterTurn)
                .arg(gae.launcher);
        // update HOT symbol
        if (gae.allAtkEffects.statsName == STATS_HP &&
            EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
            gae.allAtkEffects.value > 0 && gae.allAtkEffects.nbTurns > 1) {
          nbs.hot.nb++;
          nbs.hot.effects.push_back(effect);
        }
        // update DOT symbol
        else if (gae.allAtkEffects.statsName == STATS_HP &&
                 EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                 gae.allAtkEffects.value < 0 && gae.allAtkEffects.nbTurns > 1) {
          nbs.dot.nb++;
          nbs.dot.effects.push_back(effect);
        }
        // update buf symbol
        else if (gae.allAtkEffects.value > 0 &&
                 !(EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                   gae.allAtkEffects.nbTurns == 1)) {
          nbs.buf.nb++;
          nbs.buf.effects.push_back(effect);
        }
        // update debuf symbol
        else if (gae.allAtkEffects.value < 0 &&
                 !(EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                   gae.allAtkEffects.nbTurns == 1)) {
          nbs.debuf.nb++;
          nbs.debuf.effects.push_back(effect);
        } else if (EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                   gae.allAtkEffects.nbTurns == 1) {
          nbs.oneTurnEffect++;
        }
      }
    });
    return std::optional(nbs);
  }
  return std::nullopt;
}

int64_t Utils::GetRandomNb(const int64_t min, const int64_t max) {
  if (min == max) {
    return min;
  }
  return (rand() % (max - min)) + 1;
}

bool Utils::FileExists(const QString &path) {
  QFileInfo file(path);
  return file.exists() && file.isFile();
}

bool Utils::RemoveFile(const QString &path) {
  return FileExists(path) && QFile::remove(path);
}

double Utils::CalcRatio(const int val1, const int val2) {
    return (val2 > 0) ? static_cast<double>(val1) / static_cast<double>(val2) : 1;
}
