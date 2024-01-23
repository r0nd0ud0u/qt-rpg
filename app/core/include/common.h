#ifndef COMMON_H
#define COMMON_H

#include <qobject.h>

enum class StatsEnum {
  hp = 0,
  mana,
  vigor,
  bersecker,
  armPhy,
  armMag,
  powPhy,
  powMag,
  aggro,
  speed,
  criticalStrike,
  dodge,
  regenHp,
  regenMana,
  regenVigor
};

template <class T> class StatsType {
public:
  StatsType(StatsEnum type) : m_Type(type) {}
  T m_Value;
  StatsEnum m_Type;
};

struct Stats {
  StatsType<int> m_HP = StatsType<int>(StatsEnum::hp);
  StatsType<int> m_Mana = StatsType<int>(StatsEnum::mana);
  StatsType<int> m_Vigor = StatsType<int>(StatsEnum::vigor);
  StatsType<int> m_Bersecker = StatsType<int>(StatsEnum::bersecker);
  StatsType<int> m_ArmPhy = StatsType<int>(StatsEnum::armPhy);
  StatsType<int> m_ArmMag = StatsType<int>(StatsEnum::armMag);
  StatsType<int> m_powPhy = StatsType<int>(StatsEnum::powPhy);
  StatsType<double> m_powMag = StatsType<double>(StatsEnum::powMag);
  StatsType<int> m_aggro = StatsType<int>(StatsEnum::aggro);
  StatsType<int> m_speed = StatsType<int>(StatsEnum::criticalStrike);
  // critical strike in %
  StatsType<int> m_CriticalStrike = StatsType<int>(StatsEnum::speed);
  // dodge in %
  StatsType<double> m_dogde = StatsType<double>(StatsEnum::dodge);
  StatsType<int> m_regenHP = StatsType<int>(StatsEnum::regenHp);
  StatsType<double> m_regenMana = StatsType<double>(StatsEnum::regenMana);
  StatsType<int> m_regenVigueur = StatsType<int>(StatsEnum::regenVigor);
};

const QString OFFLINE_IMG = "./offlines/attak/img/";
const QString OFFLINE_ATK = "./offlines/attak/";

// json keys
const QString ATK_NAME = "Nom";
const QString ATK_TARGET = "Cible";
const QString ATK_REACH = "Portée";
const QString ATK_DURATION = "Durée";
const QString ATK_MANA_COST = "Coût de mana";
const QString ATK_VIGOR_COST = "Coût de vigueur";
const QString ATK_BERSECK_AGGRO = "Rage aggro";
const QString ATK_PHOTO = "Photo";
const QString ATK_DAMAGE = "Dégâts";
const QString ATK_HEAL = "Soin";
const QString ATK_REGEN_MANA = "Regen mana";
const QString ATK_EFFECT = "Effet";
const QString REACH_ZONE = "Zone";
const QString REACH_INDIVIDUAL = "Individuel";
const QString TARGET_ENNEMY = "Ennemie";
const QString TARGET_ALLY = "Allié";

#endif // COMMON_H
