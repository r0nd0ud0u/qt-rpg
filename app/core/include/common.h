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
  explicit StatsType(StatsEnum type) : m_Type(type) {}
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
const QString OFFLINE_WEARING_EQUIPMENT = "./offlines/equipment/Personnages/";
const QString OFFLINE_ROOT_EQUIPMENT = "./offlines/equipment/corps/";

// json keys
// ATK keys
const QString ATK_NAME = "Nom";
const QString ATK_TARGET = "Cible";
const QString ATK_REACH = "Portée";
const QString ATK_DURATION = "Durée";
const QString ATK_MANA_COST = "Coût de mana";
const QString ATK_VIGOR_COST = "Coût de vigueur";
const QString ATK_BERSECK_COST = "Coût de rage";
const QString ATK_BERSECK_AGGRO = "Rage aggro";
const QString ATK_PHOTO = "Photo";
const QString ATK_DAMAGE = "Dégâts";
const QString ATK_HEAL = "Soin";
const QString ATK_REGEN_MANA = "Regen mana";
const QString ATK_EFFECT = "Effet";
// Reach keys
const QString REACH_ZONE = "Zone";
const QString REACH_INDIVIDUAL = "Individuel";
// Target keys
const QString TARGET_ENNEMY = "Ennemie";
const QString TARGET_ALLY = "Allié";
const QString TARGET_ALL_HEROES = "Tous les heroes";
// Stats keys
const QString STATS_HP = "PV";
const QString STATS_MANA = "Mana";
const QString STATS_VIGOR = "Vigor";
const QString STATS_BERSECK = "Rage";
const QString STATS_ARM_PHY = "Armure physique";
const QString STATS_ARM_MAG = "Armure magique";
const QString STATS_POW_PHY = "Pouvoir physique";
const QString STATS_POW_MAG = "Pouvoir magique";
const QString STATS_AGGRO = "Aggro";
const QString STATS_SPEED = "Vitesse";
const QString STATS_CRIT = "Cout critique";
const QString STATS_DODGE = "Esquive";
const QString STATS_REGEN_HP = "Regeneration PV";
const QString STATS_REGEN_MANA = "Regeneration Mana";
const QString STATS_REGEN_VIGOR = "Regeneration vigueur";
// equipment keys
const QString EQUIP_HEAD = "Tete";
const QString EQUIP_NECKLACE = "Collier";
const QString EQUIP_CHEST = "Torse";
const QString EQUIP_PANTS = "Pantalon";
const QString EQUIP_SHOES = "Chaussures";
const QString EQUIP_LEFT_ARM = "Bras gauche";
const QString EQUIP_RIGHT_ARM = "Bras droit";
const QString EQUIP_LEFT_LEG = "Jambe gauche";
const QString EQUIP_RIGHT_LEG = "Jambe droite";
const QString EQUIP_RING = "Anneau";
const QString EQUIP_NAME = "Nom";
const QString EQUIP_RIGHT_WEAPON = "Arme gauche";
const QString EQUIP_LEFT_WEAPON = "Arme droite";
const QString EQUIP_CATEGORY = "Categorie";

#endif // COMMON_H
