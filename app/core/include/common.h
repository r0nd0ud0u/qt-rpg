#ifndef COMMON_H
#define COMMON_H

#include <qobject.h>
#include <set>
#include <unordered_map>

#include "rust-rpg-bridge/buffers.h"

const QString OFFLINE_IMG = "./offlines/attak/img/";
const QString OFFLINE_ATK = "./offlines/attak/";
const QString OFFLINE_WEARING_EQUIPMENT = "./offlines/equipment/Personnages/";
const QString OFFLINE_ROOT_EQUIPMENT = "./offlines/equipment/corps/";
const QString OFFLINE_RAND_NAME_STUFF = "./offlines/equipment/random/";

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
const QString ATK_FORM = "Forme";
// Reach keys
const QString REACH_ZONE = "Zone";
const QString REACH_INDIVIDUAL = "Individuel";
const QString REACH_RAND_INDIVIDUAL = "Aleatoire Individuel";
const std::set<QString> ALL_REACH = {"", REACH_ZONE, REACH_INDIVIDUAL,
                                     REACH_RAND_INDIVIDUAL};
// Target keys
const QString TARGET_ENNEMY = "Ennemie";
const QString TARGET_ALLY = "Allié";
const QString TARGET_ONLY_ALLY = "Seulement les alliés";
const QString TARGET_ALL_HEROES = "Tous les heroes";
const QString TARGET_HIMSELF = "Soi-même";
const std::set<QString> ALL_TARGETS = {
    "",
    TARGET_ENNEMY,
    TARGET_ALLY,
    TARGET_ALL_HEROES,
    TARGET_HIMSELF,
    TARGET_ONLY_ALLY,
};
const std::set<QString> ALLIES_TARGETS = {
    TARGET_ALLY,
    TARGET_ALL_HEROES,
    TARGET_HIMSELF,
    TARGET_ONLY_ALLY,
};
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
const QString STATS_REGEN_SPEED = "Regen vitesse";

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
                                     STATS_RATE_AGGRO,
                                     STATS_REGEN_SPEED};
const std::set<QString> ON_PERCENT_STATS = {STATS_MANA, STATS_VIGOR};
const std::set<QString> STATS_TO_LEVEL_UP = {STATS_HP, STATS_MANA, STATS_VIGOR,
                                             STATS_POW_PHY, STATS_POW_MAG};
// equipment keys
const QString EQUIP_HEAD = "Tete";
const QString EQUIP_NECKLACE = "Collier";
const QString EQUIP_CHEST = "Torse";
const QString EQUIP_PANTS = "Pantalon";
const QString EQUIP_SHOES = "Chaussures";
const QString EQUIP_ARM = "Bras";
const QString EQUIP_RING = "Anneau";
const QString EQUIP_NAME = "Nom";
const QString EQUIP_UNIQUE_NAME = "Nom unique";
const QString EQUIP_RIGHT_WEAPON = "Arme gauche";
const QString EQUIP_LEFT_WEAPON = "Arme droite";
const QString EQUIP_CATEGORY = "Categorie";
const QString EQUIP_RUNIQUE_TATOO_1 = "Tatouage 1";
const QString EQUIP_RUNIQUE_TATOO_2 = "Tatouage 2";
const QString EQUIP_RUNIQUE_TATOO_3 = "Tatouage 3";
const std::set<QString> ALL_EQUIP{EQUIP_HEAD,
                                  EQUIP_NECKLACE,
                                  EQUIP_CHEST,
                                  EQUIP_SHOES,
                                  EQUIP_ARM,
                                  EQUIP_RING,
                                  EQUIP_PANTS,
                                  EQUIP_NAME,
                                  EQUIP_RIGHT_WEAPON,
                                  EQUIP_LEFT_WEAPON,
                                  EQUIP_CATEGORY,
                                  EQUIP_RUNIQUE_TATOO_1,
                                  EQUIP_RUNIQUE_TATOO_2,
                                  EQUIP_RUNIQUE_TATOO_3};
const std::set<QString> ALL_EQUIP_ON_BODY{"",
                                          EQUIP_HEAD,
                                          EQUIP_NECKLACE,
                                          EQUIP_CHEST,
                                          EQUIP_SHOES,
                                          EQUIP_ARM,
                                          EQUIP_RING,
                                          EQUIP_PANTS,
                                          EQUIP_RIGHT_WEAPON,
                                          EQUIP_LEFT_WEAPON,
                                          EQUIP_RUNIQUE_TATOO_1,
                                          EQUIP_RUNIQUE_TATOO_2,
                                          EQUIP_RUNIQUE_TATOO_3};
const std::vector<QString> RAND_EQUIP_ON_BODY{
                                          EQUIP_HEAD,
                                          EQUIP_NECKLACE,
                                          EQUIP_CHEST,
                                          EQUIP_SHOES,
                                          EQUIP_ARM,
                                          EQUIP_RING,
                                          EQUIP_PANTS,
                                          EQUIP_RIGHT_WEAPON,
                                          EQUIP_LEFT_WEAPON,
                                          };
// Effect keys
const QString EFFECT_REINIT = "Reinit";
const QString EFFECT_NB_COOL_DOWN = "Tours de recharge";
const QString EFFECT_NB_DECREASE_ON_TURN = "Decroissement pendant le tour";
const QString EFFECT_NB_DECREASE_BY_TURN = "Decroissement par tour";
// Effect to improve cuurent value of a stat by value
const QString EFFECT_VALUE_CHANGE = "Changement par valeur";
// Effect to improve cuurent value of a stat by percent
const QString EFFECT_PERCENT_CHANGE = "Changement par %";
const QString EFFECT_DELETE_BAD = "Supprime effet néfaste";
const QString EFFECT_INTO_DAMAGE = "% (stats) en dégâts";
const QString EFFECT_IMPROVE_HOTS = "Boost chaque HOT de .. %";
const QString EFFECT_BOOSTED_BY_HOTS =
    "Boost l'effet par nb HOTS presents en %";
const QString EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT = "Up/down degats en %";
// Assess the nob of applies for a stat
const QString EFFECT_REPEAT_AS_MANY_AS = "Répète tant que possible";
// Effect to improve max value of a stat by percent (current value is updated by
// ratio)
const QString EFFECT_IMPROVE_BY_PERCENT_CHANGE = "Up par %";
// Effect to improve max value of a stat by value (current value is updated by
// ratio)
const QString EFFECT_IMPROVEMENT_STAT_BY_VALUE = "Up par valeur";
const QString EFFECT_NEXT_HEAL_IS_CRIT = "Prochaine attaque heal est crit";
const QString EFFECT_BUF_MULTI = "Buf multi";
const QString EFFECT_BLOCK_HEAL_ATK = "Bloque attaque de soin";
// Conditions on effect
// Atk is enable if damages are been tx during previous turn
const QString EFFECT_COND_DMG_PREV_TURN =
    "Active effet si Dégâts au tour précédent";
// Condition on atk
// Atk is enable if damages are been tx during previous turn
const QString CONDITION_DMG_PREV_TURN = "Dégâts au tour précédent";
const QString CONDITION_ENNEMIES_DIED = "Ennemis morts tours précédents";
const QString EFFECT_REPEAT_IF_HEAL =
    "Répète l'attaque(en % de chance) après heal tour prec.";
const QString EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL =
    "Buf par valeur d'autant de PV";
// Effect to modify the regen rate of stats such as vigor, mana, speed, hp,
// berseck by value.
const QString EFFECT_UPDATE_TURN_RATE = "Update regen par valeur";
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
                                EFFECT_IMPROVEMENT_STAT_BY_VALUE,
                                EFFECT_BUF_MULTI,
                                EFFECT_BLOCK_HEAL_ATK,
                                CONDITION_DMG_PREV_TURN,
                                EFFECT_REPEAT_IF_HEAL,
                                EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL,
                                EFFECT_UPDATE_TURN_RATE,
                                EFFECT_COND_DMG_PREV_TURN};
const std::set<QString> ACTIVE_EFFECTS_ON_LAUNCH = {
    EFFECT_NB_DECREASE_BY_TURN,
    EFFECT_NB_COOL_DOWN,
    EFFECT_REINIT,
    EFFECT_DELETE_BAD,
    EFFECT_IMPROVE_HOTS,
    EFFECT_BOOSTED_BY_HOTS,
    EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT,
    EFFECT_IMPROVEMENT_STAT_BY_VALUE,
    EFFECT_IMPROVE_BY_PERCENT_CHANGE,
    EFFECT_INTO_DAMAGE,
    EFFECT_NEXT_HEAL_IS_CRIT,
    EFFECT_BUF_MULTI,
    EFFECT_BLOCK_HEAL_ATK,
    EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL,
    EFFECT_UPDATE_TURN_RATE};
const std::set<QString> EFFECTS_HOT_OR_DOT = {EFFECT_VALUE_CHANGE,
                                              EFFECT_REPEAT_AS_MANY_AS};
const QString EFFECT_ARRAY = "Effet";
const QString EFFECT_TYPE = "Type";
const QString EFFECT_VALUE = "Value";
const QString EFFECT_TARGET = "Cible";
const QString EFFECT_REACH = "Portée";
const QString EFFECT_STAT = "Stat";
const QString EFFECT_ACTIVE_TURNS = "Tours actifs";
const QString EFFECT_SUB_VALUE = "Valeur de l'effet";

// Form keys
const QString ENT_FORM = "Ent";
const QString BEAR_FORM = "Ours";
const QString STANDARD_FORM = "Standard";
const std::set<QString> ALL_FORMS = {STANDARD_FORM, ENT_FORM, BEAR_FORM};

class StatsType {
public:
  StatsType() = default; // Default constructor
  explicit StatsType(QString type) : m_Type(type) {}
  int m_CurrentValue = 0;
  int m_StartingValue = 0;
  int m_MaxValue = 0;
  int m_RegenOnTurn = 0;
  int m_BaseEquipValue = 0;
  int m_RawMaxValue = 0;
  QString m_Type;
  int m_BufEffectValue = 0;
  int m_BufEffectPercent = 0;
  int m_BufEquipValue = 0;
  int m_BufEquipPercent = 0;
  void InitValues(int starting, int current, int max, int regen) {
    m_CurrentValue = current;
    m_MaxValue = max;
    m_RawMaxValue = max;
    m_RegenOnTurn = regen;
    // not processed ?
    m_StartingValue = starting;
  };
};

class Stats {
public:
  StatsType m_HP = StatsType(STATS_HP);
  StatsType m_Mana = StatsType(STATS_MANA);
  StatsType m_Vigor = StatsType(STATS_VIGOR);
  StatsType m_Berseck = StatsType(STATS_BERSECK);
  StatsType m_BerseckRate = StatsType(STATS_RATE_BERSECK);
  StatsType m_ArmPhy = StatsType(STATS_ARM_PHY);
  StatsType m_ArmMag = StatsType(STATS_ARM_MAG);
  StatsType m_PowPhy = StatsType(STATS_POW_PHY);
  StatsType m_PowMag = StatsType(STATS_POW_MAG);
  StatsType m_Aggro = StatsType(STATS_AGGRO);
  StatsType m_AggroRate = StatsType(STATS_RATE_AGGRO);
  StatsType m_Speed = StatsType(STATS_SPEED);
  // critical strike in %
  StatsType m_CriticalStrike = StatsType(STATS_CRIT);
  // dodge in %
  StatsType m_Dogde = StatsType(STATS_DODGE);
  StatsType m_RegenHP = StatsType(STATS_REGEN_HP);
  StatsType m_RegenMana = StatsType(STATS_REGEN_MANA);
  StatsType m_RegenVigor = StatsType(STATS_REGEN_VIGOR);

  std::unordered_map<QString, StatsType> m_AllStatsTable = {
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
