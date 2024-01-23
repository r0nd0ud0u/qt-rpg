#include "playersmanager.h"

void PlayersManager::InitHeroes() {

    Stats stats;
    stats.m_HP.m_Value = 605;
    stats.m_Mana.m_Value = 369;
    stats.m_Vigor.m_Value = 126;
    stats.m_Bersecker.m_Value = 0;
    stats.m_ArmPhy.m_Value = 108;
    stats.m_ArmMag.m_Value = 35;
    stats.m_powPhy.m_Value = 10;
    stats.m_powMag.m_Value = 137.5;
    stats.m_aggro.m_Value = 0;
    stats.m_speed.m_Value = 25;
    stats.m_CriticalStrike.m_Value = 20;
    stats.m_dogde.m_Value = 10;
    stats.m_regenHP.m_Value = 14;
    stats.m_regenMana.m_Value = 40.2;
    stats.m_regenVigueur.m_Value = 5;
    const auto hero1 = new Character("Thalia", characType::Hero, stats);

    stats.m_HP.m_Value = 600;
    stats.m_Mana.m_Value = 141;
    stats.m_Vigor.m_Value = 445;
    stats.m_Bersecker.m_Value = 0;
    stats.m_ArmPhy.m_Value = 60;
    stats.m_ArmMag.m_Value = 47;
    stats.m_powPhy.m_Value = 115;
    stats.m_powMag.m_Value = 15;
    stats.m_aggro.m_Value = 0;
    stats.m_speed.m_Value = 20;
    stats.m_CriticalStrike.m_Value = 20;
    stats.m_dogde.m_Value = 9.5;
    stats.m_regenHP.m_Value = 10;
    stats.m_regenMana.m_Value = 12;
    stats.m_regenVigueur.m_Value = 10;
    const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

    stats.m_HP.m_Value = 828;
    stats.m_Mana.m_Value = 0;
    stats.m_Vigor.m_Value = 0;
    stats.m_Bersecker.m_Value = 20;
    stats.m_ArmPhy.m_Value = 85;
    stats.m_ArmMag.m_Value = 45;
    stats.m_powPhy.m_Value = 30;
    stats.m_powMag.m_Value = 0;
    stats.m_aggro.m_Value = 0;
    stats.m_speed.m_Value = 14;
    stats.m_CriticalStrike.m_Value = 15;
    stats.m_dogde.m_Value = 5;
    stats.m_regenHP.m_Value = 0;
    stats.m_regenMana.m_Value = 0;
    stats.m_regenVigueur.m_Value = 0;
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
    stats.m_HP.m_Value = 9999;
    stats.m_Mana.m_Value = 0;
    stats.m_Vigor.m_Value = 126;
    stats.m_Bersecker.m_Value = 0;
    stats.m_ArmPhy.m_Value = 2280;
    stats.m_ArmMag.m_Value = 35;
    stats.m_powPhy.m_Value = 10;
    stats.m_powMag.m_Value = 138;
    stats.m_aggro.m_Value = 0;
    stats.m_speed.m_Value = 25;
    stats.m_CriticalStrike.m_Value = 20;
    stats.m_dogde.m_Value = 10;
    stats.m_regenHP.m_Value = 4;
    stats.m_regenMana.m_Value = 25;
    stats.m_regenVigueur.m_Value = 5;
    const auto boss = new Character("Pignouf hehe", characType::Boss, stats);

    m_BossesList.push_back(boss);
}
