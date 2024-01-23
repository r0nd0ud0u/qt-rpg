#include "playersmanager.h"

void PlayersManager::InitHeroes() {

    Stats stats;
    stats.m_HP = 605;
    stats.m_Mana = 369;
    stats.m_Vigor = 126;
    stats.m_Bersecker = 0;
    stats.m_ArmPhy = 80;
    stats.m_ArmMag = 35;
    stats.m_powPhy = 10;
    stats.m_powMag = 138;
    stats.m_aggro = 0;
    stats.m_speed = 25;
    stats.m_CriticalStrike = 20;
    stats.m_dogde = 10;
    stats.m_regenHP = 4;
    stats.m_regenMana = 25;
    stats.m_regenVigueur = 5;
    const auto hero1 = new Character("Thalia", characType::Hero, stats);

    stats.m_HP = 600;
    stats.m_Mana = 141;
    stats.m_Vigor = 445;
    stats.m_Bersecker = 0;
    stats.m_ArmPhy = 60;
    stats.m_ArmMag = 47;
    stats.m_powPhy = 115;
    stats.m_powMag = 15;
    stats.m_aggro = 0;
    stats.m_speed = 20;
    stats.m_CriticalStrike = 20;
    stats.m_dogde = 9.5;
    stats.m_regenHP = 10;
    stats.m_regenMana = 12;
    stats.m_regenVigueur = 10;
    const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

    stats.m_HP = 828;
    stats.m_Mana = 0;
    stats.m_Vigor = 0;
    stats.m_Bersecker = 20;
    stats.m_ArmPhy = 85;
    stats.m_ArmMag = 45;
    stats.m_powPhy = 30;
    stats.m_powMag = 0;
    stats.m_aggro = 0;
    stats.m_speed = 14;
    stats.m_CriticalStrike = 15;
    stats.m_dogde = 5;
    stats.m_regenHP = 0;
    stats.m_regenMana = 0;
    stats.m_regenVigueur = 0;
    const auto hero3 = new Character("Thraïn", characType::Hero, stats);

    m_HeroesList.push_back(hero1);
    m_HeroesList.push_back(hero2);
    m_HeroesList.push_back(hero3);

    for(const auto& hero : m_HeroesList){
        hero->LoadAtkJson();
    }
}

void PlayersManager::InitBosses() {
    Stats stats;
    stats.m_HP = 9999;
    stats.m_Mana = 0;
    stats.m_Vigor = 126;
    stats.m_Bersecker = 0;
    stats.m_ArmPhy = 2280;
    stats.m_ArmMag = 35;
    stats.m_powPhy = 10;
    stats.m_powMag = 138;
    stats.m_aggro = 0;
    stats.m_speed = 25;
    stats.m_CriticalStrike = 20;
    stats.m_dogde = 10;
    stats.m_regenHP = 4;
    stats.m_regenMana = 25;
    stats.m_regenVigueur = 5;
    const auto boss = new Character("Pignouf hehe", characType::Boss, stats);

    m_BossesList.push_back(boss);
}
