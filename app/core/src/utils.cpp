#include "utils.h"

#include <iomanip>
#include <chrono>
#include <ctime> // for std::localtime

// Define a comparison function for sorting based on the 'a' member
bool Utils::CompareByLevel(const AttaqueType &obj1, const AttaqueType &obj2) {
  return obj1.level < obj2.level;
}

QString Utils::getCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(local_tm, "%Y-%m-%d-%H-%M-%S");
    return QString::fromStdString(oss.str());
}
