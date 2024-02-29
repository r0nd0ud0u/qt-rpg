#ifndef COMMON_H
#define COMMON_H

#include <qobject.h>
#include <set>
#include <unordered_map>
#include <variant>

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
const QString ATK_AGGRO = "Aggro";
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
const std::set<QString> ALL_REACH = {"", REACH_ZONE, REACH_INDIVIDUAL};
// Target keys
const QString TARGET_ENNEMY = "Ennemie";
const QString TARGET_ALLY = "Allié";
const QString TARGET_ONLY_ALLY = "Seulement les alliés";
const QString TARGET_ALL_HEROES = "Tous les heroes";
const QString TARGET_HIMSELF = "Soi-même";
const std::set<QString> ALL_TARGETS = {"",
                                       TARGET_ENNEMY,
                                       TARGET_ALLY,
                                       TARGET_ALL_HEROES,
                                       TARGET_HIMSELF,
                                       TARGET_ONLY_ALLY};
const std::set<QString> ALLIES_TARGETS = {TARGET_ALLY, TARGET_ALL_HEROES,
                                          TARGET_HIMSELF, TARGET_ONLY_ALLY};
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
const QString STATS_RATE_BERSECK = "Taux rage";
const QString STATS_RATE_AGGRO = "Taux aggro";

const std::set<QString> ALL_STATS = {"",
                                     STATS_HP,
                                     STATS_MANA,
                                     STATS_VIGOR,
                                     STATS_BERSECK,
                                     STATS_ARM_PHY,
                                     STATS_ARM_MAG,
                                     STATS_POW_PHY,
                                     STATS_POW_MAG,
                                     STATS_AGGRO,
                                     STATS_SPEED,
                                     STATS_CRIT,
                                     STATS_DODGE,
                                     STATS_REGEN_HP,
                                     STATS_REGEN_MANA,
                                     STATS_REGEN_VIGOR,
                                     STATS_RATE_BERSECK,
                                     STATS_RATE_AGGRO};
const std::set<QString> ON_PERCENT_STATS = {STATS_MANA, STATS_VIGOR};
const std::set<QString> STATS_TO_LEVEL_UP = {STATS_HP,      STATS_MANA,
                                             STATS_VIGOR,   STATS_BERSECK,
                                             STATS_POW_PHY, STATS_POW_MAG};
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
const std::set<QString> ALL_EQUIP{
    EQUIP_HEAD,        EQUIP_NECKLACE,  EQUIP_CHEST,    EQUIP_SHOES,
    EQUIP_LEFT_ARM,    EQUIP_RIGHT_ARM, EQUIP_LEFT_LEG, EQUIP_RIGHT_LEG,
    EQUIP_RING,        EQUIP_PANTS,     EQUIP_NAME,     EQUIP_RIGHT_WEAPON,
    EQUIP_LEFT_WEAPON, EQUIP_CATEGORY};
const std::set<QString> ALL_EQUIP_ON_BODY{"",
                                          EQUIP_HEAD,
                                          EQUIP_NECKLACE,
                                          EQUIP_CHEST,
                                          EQUIP_SHOES,
                                          EQUIP_LEFT_ARM,
                                          EQUIP_RIGHT_ARM,
                                          EQUIP_LEFT_LEG,
                                          EQUIP_RIGHT_LEG,
                                          EQUIP_RING,
                                          EQUIP_PANTS,
                                          EQUIP_RIGHT_WEAPON,
                                          EQUIP_LEFT_WEAPON};
// Effect keys
const QString EFFECT_REINIT = "Reinit";
const QString EFFECT_NB_COOL_DOWN = "Tours de recharge";
const QString EFFECT_NB_DECREASE_ON_TURN = "Decroissement pendant le tour";
const QString EFFECT_NB_DECREASE_BY_TURN = "Decroissement par tour";
const QString EFFECT_VALUE_CHANGE = "Changement par valeur";
const QString EFFECT_PERCENT_CHANGE = "Changement par %";
const QString EFFECT_IMPROVE_BY_PERCENT_CHANGE = "Up par %";
const QString EFFECT_DELETE_BAD = "Supprime effet néfaste";
const QString EFFECT_INTO_DAMAGE = "% (stats) en dégâts";
const QString EFFECT_IMPROVE_HOTS = "Boost chaque HOT de .. %";
const QString EFFECT_BOOSTED_BY_HOTS =
    "Boost l'effet par nb HOTS presents en %";
const QString EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT = "Up/down degats en %";
const QString EFFECT_REPEAT_AS_MANY_AS = "Répète tant que possible";
const QString CONDITION_ENNEMIES_DIED = "Ennemis morts tours précédents";
const QString EFFECT_IMPROVEMENT_STAT_BY_VALUE = "Up par valeur";
const std::set<QString> EFFECTS{"",
                                EFFECT_REINIT,
                                EFFECT_NB_COOL_DOWN,
                                EFFECT_NB_DECREASE_ON_TURN,
                                EFFECT_NB_DECREASE_BY_TURN,
                                EFFECT_VALUE_CHANGE,
                                EFFECT_PERCENT_CHANGE,
                                EFFECT_DELETE_BAD,
                                EFFECT_INTO_DAMAGE,
                                EFFECT_IMPROVE_HOTS,
                                EFFECT_BOOSTED_BY_HOTS,
                                EFFECT_IMPROVE_BY_PERCENT_CHANGE,
                                EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT,
                                EFFECT_REPEAT_AS_MANY_AS,
                                CONDITION_ENNEMIES_DIED,
                                EFFECT_IMPROVEMENT_STAT_BY_VALUE};
const std::set<QString> ACTIVE_EFFECTS_ON_LAUNCH = {
    EFFECT_NB_DECREASE_BY_TURN,
    EFFECT_NB_COOL_DOWN,
    EFFECT_NB_DECREASE_ON_TURN,
    EFFECT_REINIT,
    EFFECT_DELETE_BAD,
    EFFECT_IMPROVE_HOTS,
    EFFECT_BOOSTED_BY_HOTS,
    EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT,
    EFFECT_IMPROVEMENT_STAT_BY_VALUE

};
const QString EFFECT_ARRAY = "Effet";
const QString EFFECT_TYPE = "Type";
const QString EFFECT_VALUE = "Value";
const QString EFFECT_TARGET = "Cible";
const QString EFFECT_REACH = "Portée";
const QString EFFECT_STAT = "Stat";
const QString EFFECT_ACTIVE_TURNS = "Tours actifs";
const QString EFFECT_SUB_VALUE = "Valeur de l'effet";

struct Buf {
  int m_Value = 0;
  bool m_IsPercent = false;
  void SetBuf(const int value, const bool isPercent) {
    m_Value = value;
    m_IsPercent = isPercent;
  }
};

template <class T> class StatsType {
public:
  StatsType() = default; // Default constructor
  explicit StatsType(QString type) : m_Type(type) {}
  T m_CurrentValue;
  T m_StartingValue;
  T m_MaxValue;
  T m_RegenOnTurn;
  T m_BaseEquipValue;
  T m_RawMaxValue;
  QString m_Type;
  T m_BufEffectValue;
  int m_BufEffectPercent;
  T m_BufEquipValue;
  int m_BufEquipPercent;
  void InitValues(T starting, T current, T max, T regen) {
    m_CurrentValue = current;
    m_MaxValue = max;
    m_BufEffectPercent = 0;
    m_BufEquipPercent = 0;
    m_BaseEquipValue = 0;
    m_BufEquipValue = 0;
    m_BufEffectValue = 0;
    m_RawMaxValue = max;

    m_RegenOnTurn = regen;
    // not processed ?
    m_StartingValue = starting;
  };
};

using statsVariant = std::variant<StatsType<double>, StatsType<int>>;

class Stats {
public:
  StatsType<int> m_HP = StatsType<int>(STATS_HP);
  StatsType<int> m_Mana = StatsType<int>(STATS_MANA);
  StatsType<int> m_Vigor = StatsType<int>(STATS_VIGOR);
  StatsType<int> m_Berseck = StatsType<int>(STATS_BERSECK);
  StatsType<int> m_BerseckRate = StatsType<int>(STATS_RATE_BERSECK);
  StatsType<int> m_ArmPhy = StatsType<int>(STATS_ARM_PHY);
  StatsType<int> m_ArmMag = StatsType<int>(STATS_ARM_MAG);
  StatsType<int> m_PowPhy = StatsType<int>(STATS_POW_PHY);
  StatsType<int> m_PowMag = StatsType<int>(STATS_POW_MAG);
  StatsType<int> m_Aggro = StatsType<int>(STATS_AGGRO);
  StatsType<int> m_AggroRate = StatsType<int>(STATS_RATE_AGGRO);
  StatsType<int> m_Speed = StatsType<int>(STATS_SPEED);
  // critical strike in %
  StatsType<int> m_CriticalStrike = StatsType<int>(STATS_CRIT);
  // dodge in %
  StatsType<int> m_Dogde = StatsType<int>(STATS_DODGE);
  StatsType<int> m_RegenHP = StatsType<int>(STATS_REGEN_HP);
  StatsType<int> m_RegenMana = StatsType<int>(STATS_REGEN_MANA);
  StatsType<int> m_RegenVigor = StatsType<int>(STATS_REGEN_VIGOR);

  std::unordered_map<QString, statsVariant> m_AllStatsTable = {
      {STATS_HP, m_HP},
      {STATS_MANA, m_Mana},
      {STATS_VIGOR, m_Vigor},
      {STATS_BERSECK, m_Berseck},
      {STATS_RATE_BERSECK, m_BerseckRate},
      {STATS_ARM_PHY, m_ArmPhy},
      {STATS_ARM_MAG, m_ArmMag},
      {STATS_POW_PHY, m_PowPhy},
      {STATS_POW_MAG, m_PowMag},
      {STATS_AGGRO, m_Aggro},
      {STATS_RATE_AGGRO, m_AggroRate},
      {STATS_SPEED, m_Speed},
      {STATS_CRIT, m_CriticalStrike},
      {STATS_DODGE, m_Dogde},
      {STATS_REGEN_HP, m_RegenHP},
      {STATS_REGEN_MANA, m_RegenMana},
      {STATS_REGEN_VIGOR, m_RegenVigor}};
};

#endif // COMMON_H
