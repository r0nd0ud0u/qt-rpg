#include "utils.h"

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
