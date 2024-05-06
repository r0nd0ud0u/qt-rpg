#include "utils.h"

#include "character.h"
#include "playersmanager.h"

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
 * @brief Utils::GetNbOfActiveEffects
 * Returns the number of hot, dot, buf and dot for all the effects in one attak.
 * The number of one-turn effect (damage or heal etc., change by value and %) is
 * assessed.
 */
std::optional<EffectsTypeNb>
Utils::GetNbOfActiveEffects(const std::vector<GameAtkEffects> &table) {
  EffectsTypeNb nbs;
  // for all hot/dot and buf/debuf updates
  // hot dot symbols update
  if (!table.empty()) {
    std::for_each(table.begin(), table.end(), [&](const GameAtkEffects &gae) {
      if (!gae.allAtkEffects.passiveTalent) {
        // update HOT symbol
        if (gae.allAtkEffects.statsName == STATS_HP &&
            EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
            gae.allAtkEffects.value > 0 && gae.allAtkEffects.nbTurns > 1) {
          nbs.hot++;
        }
        // update DOT symbol
        else if (gae.allAtkEffects.statsName == STATS_HP &&
                 EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                 gae.allAtkEffects.value > 0 && gae.allAtkEffects.nbTurns > 1) {
          nbs.dot++;
        }
        // update buf symbol
        else if (gae.allAtkEffects.value > 0 &&
                 !(EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                   gae.allAtkEffects.nbTurns == 1)) {
          nbs.buf++;
        }
        // update debuf symbol
        else if (gae.allAtkEffects.value < 0 &&
                 !(EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                   gae.allAtkEffects.nbTurns == 1)) {
          nbs.debuf++;
        } else if (EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0 &&
                   gae.allAtkEffects.nbTurns == 1) {
          nbs.oneTurnHotDot++;
        }
      }
    });
    return std::optional(nbs);
  }
  return std::nullopt;
}
