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
    AttaqueType atq1;
    atq1.name = "atq1";
    atq1.damage = 100;
    atq1.effect = "effect";
    hero1->AddAtq(atq1);
    AttaqueType atq2;
    atq2.name = "atq2";
    atq2.damage = 100;
    atq2.effect = "effect";
    hero1->AddAtq(atq2);

    stats.m_HP = 605;
    stats.m_Mana = 1369;
    stats.m_Vigor = 1126;
    stats.m_Bersecker = 10;
    stats.m_ArmPhy = 2280;
    stats.m_ArmMag = 135;
    stats.m_powPhy = 110;
    stats.m_powMag = 1138;
    stats.m_aggro = 10;
    stats.m_speed = 125;
    stats.m_CriticalStrike = 10;
    stats.m_dogde = 110;
    stats.m_regenHP = 14;
    stats.m_regenMana = 215;
    stats.m_regenVigueur = 11;
    const auto hero2 = new Character("Seb", characType::Hero, stats);
    atq1.name = "heheh";
    atq2.name = "hahaa";
    hero2->AddAtq(atq1);
    hero2->AddAtq(atq2);

    stats.m_HP = 2605;
    stats.m_Mana = 2369;
    stats.m_Vigor = 2126;
    stats.m_Bersecker = 20;
    stats.m_ArmPhy = 12180;
    stats.m_ArmMag = 235;
    stats.m_powPhy = 210;
    stats.m_powMag = 2138;
    stats.m_aggro = 20;
    stats.m_speed = 225;
    stats.m_CriticalStrike = 220;
    stats.m_dogde = 210;
    stats.m_regenHP = 24;
    stats.m_regenMana = 225;
    stats.m_regenVigueur = 25;
    const auto hero3 = new Character("Marc", characType::Hero, stats);
    atq1.name = "hihih";
    atq2.name = "hohoh";
    hero3->AddAtq(atq1);
    hero3->AddAtq(atq2);

    m_HeroesList.push_back(hero1);
    m_HeroesList.push_back(hero2);
    m_HeroesList.push_back(hero3);
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
