#ifndef BOSSCLASS_H
#define BOSSCLASS_H

#include "common.h"

const std::set<QString> hehe{};

class BossClass
{
public:
    BossClass();
    int64_t m_Rank = 0;

    static std::vector<std::vector<uint64_t>> PROBA_LOOTS;
    static std::vector<uint64_t> ARMOR;
    static std::unordered_map<QString, std::vector<Buffers*>> BONUS_LIST;
    static std::vector<Buffers*> GetBuffer(const bool isPercent, const int initValue, const int delta);
};

#endif // BOSSCLASS_H
