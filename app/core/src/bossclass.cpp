#include "bossclass.h"

// Values: {No loot, Commun, Rare, Epic, Legendary}
std::vector<std::vector<uint64_t>> BossClass::PROBA_LOOTS{
    std::vector<uint64_t>{100, 0, 0, 0, 0}, // Rank 0
    std::vector<uint64_t>{0, 60, 30, 10, 0},
    std::vector<uint64_t>{0, 30, 40, 30, 0},
    std::vector<uint64_t>{0, 20, 30, 40, 10},
    std::vector<uint64_t>{0, 0, 20, 55, 25},
    std::vector<uint64_t>{0, 0, 0, 40, 60}};

std::unordered_map<QString, std::vector<Buffers *>> BossClass::BONUS_LIST{
    {STATS_HP, GetBuffer(true, 3, 4)},
    {STATS_REGEN_HP, GetBuffer(false, 2, 1)},
    {STATS_MANA, GetBuffer(true, 3, 4)},
    {STATS_REGEN_MANA, GetBuffer(false, 2, 1)},
    {STATS_VIGOR, GetBuffer(true, 3, 4)},
    {STATS_REGEN_VIGOR, GetBuffer(false, 2, 1)},
    {STATS_BERSECK, GetBuffer(true, 3, 4)},
    {STATS_RATE_BERSECK, GetBuffer(false, 1, 1)},
    {STATS_ARM_MAG, GetBuffer(false, 25, 25)},
    {STATS_ARM_PHY, GetBuffer(false,  25, 25)},
    {STATS_POW_MAG, GetBuffer(false,  20, 20)},
    {STATS_POW_PHY, GetBuffer(false, 20, 20)},
    {STATS_DODGE, GetBuffer(false, 1, 1)},
    {STATS_REGEN_SPEED, GetBuffer(false, 2, 3)},
    {STATS_CRIT, GetBuffer(false, 2, 1)}};

std::vector<uint64_t> BossClass::ARMOR{
    25,  // Commun
    50,  // Rare
    75,  // Epic
    100, // Legendary
};

BossClass::BossClass() {}

std::vector<Buffers *> BossClass::GetBuffer(const bool isPercent,
                                            const int initValue,
                                            const int delta) {
  std::vector<Buffers *> buffers;
  for (int i = 0; i < 4; i++) {
    Buffers *buff = buffers_new().into_raw();
    buff->set_buffers(initValue + delta * i, true);
    buffers.push_back(buff);
  }

  return buffers;
}
