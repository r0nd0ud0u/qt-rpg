#include "gamemanager.h"

#include "character.h"

void GameManager::InitHeroes() {
  m_Heroes = std::make_unique<Heroes>();

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
  const auto hero2 = new Character("Thalia", characType::Hero, stats);

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
  const auto hero3 = new Character("Thalia", characType::Hero, stats);

  m_Heroes->m_HeroesList.push_back(hero1);
  m_Heroes->m_HeroesList.push_back(hero2);
  m_Heroes->m_HeroesList.push_back(hero3);
}
