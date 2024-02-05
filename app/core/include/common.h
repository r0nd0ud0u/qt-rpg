#ifndef COMMON_H
#define COMMON_H

#include <qobject.h>
#include <unordered_set>

enum class StatsEnum {
  hp = 0,
  mana,
  vigor,
  berseck,
  berseckRate,
  armPhy,
  armMag,
  powPhy,
  powMag,
  aggro,
  aggroRate,
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
  T m_CurrentValue;
  T m_StartingValue;
  T m_MaxValue;
  StatsEnum m_Type;
  void SetValues(T starting, T current, T max) {
    m_CurrentValue = current;
    m_StartingValue = starting;
    m_MaxValue = max;
  };
};

struct Stats {
  StatsType<int> m_HP = StatsType<int>(StatsEnum::hp);
  StatsType<int> m_Mana = StatsType<int>(StatsEnum::mana);
  StatsType<int> m_Vigor = StatsType<int>(StatsEnum::vigor);
  StatsType<int> m_Berseck = StatsType<int>(StatsEnum::berseck);
  StatsType<int> m_BerseckRate = StatsType<int>(StatsEnum::berseckRate);
  StatsType<int> m_ArmPhy = StatsType<int>(StatsEnum::armPhy);
  StatsType<int> m_ArmMag = StatsType<int>(StatsEnum::armMag);
  StatsType<int> m_PowPhy = StatsType<int>(StatsEnum::powPhy);
  StatsType<double> m_PowMag = StatsType<double>(StatsEnum::powMag);
  StatsType<int> m_Aggro = StatsType<int>(StatsEnum::aggro);
  StatsType<int> m_AggroRate = StatsType<int>(StatsEnum::aggroRate);
  StatsType<int> m_Speed = StatsType<int>(StatsEnum::speed);
  // critical strike in %
  StatsType<int> m_CriticalStrike = StatsType<int>(StatsEnum::criticalStrike);
  // dodge in %
  StatsType<double> m_Dogde = StatsType<double>(StatsEnum::dodge);
  StatsType<int> m_RegenHP = StatsType<int>(StatsEnum::regenHp);
  StatsType<int> m_RegenMana = StatsType<int>(StatsEnum::regenMana);
  StatsType<int> m_RegenVigor = StatsType<int>(StatsEnum::regenVigor);
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
const QString ATK_AGGRO = "Rage aggro";
const QString ATK_PHOTO = "Photo";
const QString ATK_DAMAGE = "Dégâts";
const QString ATK_HEAL = "Soin";
const QString ATK_REGEN_MANA = "Regen mana";
const QString ATK_EFFECT = "Effet";
const QString ATK_LEVEL = "Niveau";
const QString ATK_REGEN_VIGOR = "Regen vigueur";
const QString ATK_REGEN_BERSECK = "Regen rage";
// Reach keys
const QString REACH_ZONE = "Zone";
const QString REACH_INDIVIDUAL = "Individuel";
const std::unordered_set<QString> ALL_REACH = {"", REACH_ZONE, REACH_INDIVIDUAL};
// Target keys
const QString TARGET_ENNEMY = "Ennemie";
const QString TARGET_ALLY = "Allié";
const QString TARGET_ALL_HEROES = "Tous les heroes";
const QString TARGET_HIMSELF = "Soi-même";
const std::unordered_set<QString> ALL_TARGETS = {"", TARGET_ENNEMY, TARGET_ALLY, TARGET_ALL_HEROES, TARGET_HIMSELF};
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
const std::unordered_set<QString> ALL_STATS = {"",
    STATS_HP,       STATS_MANA,       STATS_VIGOR,      STATS_BERSECK,
    STATS_ARM_PHY,  STATS_ARM_MAG,    STATS_POW_PHY,    STATS_POW_MAG,
    STATS_AGGRO,    STATS_SPEED,      STATS_CRIT,       STATS_DODGE,
    STATS_REGEN_HP, STATS_REGEN_MANA, STATS_REGEN_VIGOR};
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
// Effect keys
const QString EFFECT_ON = "Effet on";
const QString EFFECT_NB_COOL_DOWN = "Tours de recharge";
const QString EFFECT_NB_DECREASE_ON_TURN = "Decroissement pendant le tour";
const QString EFFECT_NB_DECREASE_BY_TURN = "Decroissement par tour";

const QString EFFECT_VALUE_CHANGE = "Changement par valeur";
const QString EFFECT_PERCENT_CHANGE = "Changement par %";
const std::unordered_set<QString> EFFECTS{"",
                                          EFFECT_ON,
                                          EFFECT_NB_COOL_DOWN,
                                          EFFECT_NB_DECREASE_ON_TURN,
                                          EFFECT_NB_DECREASE_BY_TURN,
                                          EFFECT_VALUE_CHANGE,
                                          EFFECT_PERCENT_CHANGE};
const QString EFFECT_ARRAY = "Effet";
const QString EFFECT_TYPE = "Type";
const QString EFFECT_VALUE = "Value";
const QString EFFECT_TARGET = "Cible";
const QString EFFECT_REACH = "Portée";
const QString EFFECT_STAT = "Stat";
const QString EFFECT_ACTIVE_TURNS = "Tours actifs";

#endif // COMMON_H
