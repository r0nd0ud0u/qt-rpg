#ifndef COMMON_H
#define COMMON_H

#include <qobject.h>
#include <set>
#include <unordered_map>

// common include
#include "rust-rpg-bridge/buffers.h"

// inputs
inline const QString OFFLINE_IMG = "./offlines/attak/img/";
inline const QString OFFLINE_ATK = "./offlines/attak/";
inline const QString OFFLINE_WEARING_EQUIPMENT = "./offlines/equipment/Personnages/";
inline const QString OFFLINE_ROOT_EQUIPMENT = "./offlines/equipment/corps/";
inline const QString OFFLINE_RAND_NAME_STUFF = "./offlines/equipment/random/";
inline const QString OFFLINE_CHARACTERS = "./offlines/Personnages/";
inline const QString OFFLINE_SAVES = "./offlines/games";

// save/load games
// paths
inline const QString GAMES_DIR = "./offlines/games/";
inline const QString GAMES_CHARACTERS = "characters";
inline const QString GAMES_EQUIPMENT = "equipment";
inline const QString GAMES_EFFECTS = "effects";
inline const QString GAMES_STATE = "game-state";
inline const QString GAMES_LOOT_EQUIPMENT = "equipment/body";

// sounds
inline const QString SOUNDS_DIR = "./offlines/sounds/";

// game state
inline const QString GAME_STATE_DIED_ENNEMIES = "died-ennemies";
inline const QString GAME_STATE_ORDER_PLAYERS = "order-players-last-turn";
inline const QString GAME_STATE_CURRENT_TURN = "current-turn";
inline const QString GAME_STATE_CURRENT_ROUND = "current-round";
inline const QString GAME_STATE_GAME_NAME = "game-name";
inline const QString GAME_STATE_STATS_IN_GAME = "/stats_in_game_%1.csv";

// outputs
inline const QString OUTPUT_DIR = "./output/";
inline const QString OUTPUT_ENDGAME = "./output/endgame_%1.csv";
inline const QStringList ENDGAME_TITLE_BAR = {"Name", "Player Type", "Life status"};
// json keys
// ATK keys
inline const QString ATK_NAME = "Nom";
inline const QString ATK_TARGET = "Cible";
inline const QString ATK_REACH = "Portée";
inline const QString ATK_DURATION = "Durée";
inline const QString ATK_MANA_COST = "Coût de mana";
inline const QString ATK_VIGOR_COST = "Coût de vigueur";
inline const QString ATK_BERSECK_COST = "Coût de rage";
inline const QString ATK_AGGRO = "Aggro";
inline const QString ATK_PHOTO = "Photo";
inline const QString ATK_DAMAGE = "Dégâts";
inline const QString ATK_HEAL = "Soin";
inline const QString ATK_REGEN_MANA = "Regen mana";
inline const QString ATK_EFFECT = "Effet";
inline const QString ATK_LEVEL = "Niveau";
inline const QString ATK_REGEN_VIGOR = "Regen vigueur";
inline const QString ATK_REGEN_BERSECK = "Regen rage";
inline const QString ATK_FORM = "Forme";
inline const QString ATK_SOUND = "Sound";
// Reach keys
inline const QString REACH_ZONE = "Zone";
inline const QString REACH_INDIVIDUAL = "Individuel";
inline const QString REACH_RAND_INDIVIDUAL = "Aleatoire Individuel";
inline const std::set<QString> ALL_REACH = {"", REACH_ZONE, REACH_INDIVIDUAL,
                                     REACH_RAND_INDIVIDUAL};
// Target keys
inline const QString TARGET_ENNEMY = "Ennemie";
inline const QString TARGET_ALLY = "Allié";
inline const QString TARGET_ONLY_ALLY = "Seulement les alliés";
inline const QString TARGET_ALL_HEROES = "Tous les heroes";
inline const QString TARGET_HIMSELF = "Soi-même";
inline const std::set<QString> ALL_TARGETS = {
    "",
    TARGET_ENNEMY,
    TARGET_ALLY,
    TARGET_ALL_HEROES,
    TARGET_HIMSELF,
    TARGET_ONLY_ALLY,
};
inline const std::set<QString> ALLIES_TARGETS = {
    TARGET_ALLY,
    TARGET_ALL_HEROES,
    TARGET_HIMSELF,
    TARGET_ONLY_ALLY,
};
// Stats keys
inline const QString STATS_HP = "PV";
inline const QString STATS_MANA = "Mana";
inline const QString STATS_VIGOR = "Vigueur";
inline const QString STATS_BERSECK = "Rage";
inline const QString STATS_ARM_PHY = "Armure physique";
inline const QString STATS_ARM_MAG = "Armure magique";
inline const QString STATS_POW_PHY = "Pouvoir physique";
inline const QString STATS_POW_MAG = "Pouvoir magique";
inline const QString STATS_AGGRO = "Aggro";
inline const QString STATS_SPEED = "Vitesse";
inline const QString STATS_CRIT = "Cout critique";
inline const QString STATS_DODGE = "Esquive";
inline const QString STATS_REGEN_HP = "Regeneration PV";
inline const QString STATS_REGEN_MANA = "Regeneration Mana";
inline const QString STATS_REGEN_VIGOR = "Regeneration vigueur";
inline const QString STATS_RATE_BERSECK = "Taux rage";
inline const QString STATS_RATE_AGGRO = "Taux aggro";
inline const QString STATS_REGEN_SPEED = "Regen vitesse";

inline const std::set<QString> ALL_STATS = {"",
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

inline const std::unordered_map<QString, QString> conv = {
    {STATS_HP, "HP"},
    {STATS_MANA, "Mana"},
    {STATS_VIGOR, "Vigor"},
    {STATS_BERSECK, "Berseck"},
    {STATS_ARM_PHY, "Physical armor"},
    {STATS_ARM_MAG, "Magical armor"},
    {STATS_POW_PHY, "Physical power"},
    {STATS_POW_MAG, "Magical power"},
    {STATS_AGGRO, "Aggro"},
    {STATS_SPEED, "Speed"},
    {STATS_CRIT, "Critical strike"},
    {STATS_DODGE, "Dodge"},
    {STATS_REGEN_HP, "HP regeneration"},
    {STATS_REGEN_MANA, "Mana regeneration"},
    {STATS_REGEN_VIGOR, "Vigor regeneration"},
    {STATS_RATE_BERSECK, "Berseck rate"},
    {STATS_RATE_AGGRO, "Aggro rate"},
    {STATS_REGEN_SPEED, "Speed regeneration"},
    };


inline const std::set<QString> ON_PERCENT_STATS = {STATS_MANA, STATS_VIGOR};
inline const std::set<QString> STATS_TO_LEVEL_UP = {STATS_HP, STATS_MANA, STATS_VIGOR,
                                             STATS_POW_PHY, STATS_POW_MAG};

// Characters keys
inline const QString CH_NAME = "Nom";
inline const QString CH_SHORT_NAME = "Nom court";
inline const QString CH_PHOTO_NAME = "Photo";
inline const QString CH_TYPE = "Type";
inline const QString CH_TYPE_HERO = "Hero";
inline const QString CH_TYPE_BOSS = "Boss";
inline const QString CH_CURRENT_VALUE = "Current";
inline const QString CH_MAX_VALUE = "Max";
inline const QString CH_LEVEL = "Niveau";
inline const QString CH_EXP = "Exp";
inline const QString CH_COLOR = "Couleur";
inline const QString CH_RANK = "Rang";
inline const QString CH_FORM = "Forme";
inline const QString CH_CLASS = "Classe";
inline const QString CH_ACTIONS_IN_ROUND = "nb-actions-in-round";
inline const QString CH_BLOCKING_ATK = "is-blocking-atk";
inline const QString CH_MAX_NB_ACTIONS_ROUND = "max_actions_by_round";
// buf - debuf
inline const QString CH_BUF_DEBUF = "Buf-debuf";
inline const QString CH_BUF_ALL_STATS = "Buf-all-stats";
inline const QString CH_BUF_VALUE = "Buf-value";
inline const QString CH_BUF_PASSIVE_ENABLED = "Buf-passive-enabled";
inline const QString CH_BUF_IS_PERCENT = "Buf-is-percent";
inline const QString CH_BUF_TYPE = "Buf-type";
// last tx rx
inline const QString CH_TXRX_TYPE = "Tx-rx-type";
inline const QString CH_TXRX = "Tx-rx";
inline const QString CH_TXRX_SIZE = "Tx-rx-size";
// powers
inline const QString CH_POWERS_CRIT_AFTER_HEAL = "is_crit_heal_after_crit";
inline const QString CH_POWERS_DMG_TX_ALLY = "is_damage_tx_heal_needy_ally";
// extended character
inline const QString CH_EXT_RAND_TARGET = "is_random_target";
inline const QString CH_EXT_HEAL_ATK_BLOCKED = "is_heal_atk_blocked";
inline const QString CH_EXT_FIRST_ROUND = "is_first_round";

// equipment keys
inline const QString EQUIP_HEAD = "Tete";
inline const QString EQUIP_NECKLACE = "Collier";
inline const QString EQUIP_CHEST = "Torse";
inline const QString EQUIP_PANTS = "Pantalon";
inline const QString EQUIP_SHOES = "Chaussures";
inline const QString EQUIP_ARM = "Bras";
inline const QString EQUIP_RING_RIGHT = "Anneau droit";
inline const QString EQUIP_RING_LEFT = "Anneau gauche";
inline const QString EQUIP_NAME = "Nom";
inline const QString EQUIP_UNIQUE_NAME = "Nom unique";
inline const QString EQUIP_RIGHT_WEAPON = "Arme gauche";
inline const QString EQUIP_LEFT_WEAPON = "Arme droite";
inline const QString EQUIP_CATEGORY = "Categorie";
inline const QString EQUIP_RUNIQUE_TATOO_1 = "Tatouage 1";
inline const QString EQUIP_RUNIQUE_TATOO_2 = "Tatouage 2";
inline const QString EQUIP_RUNIQUE_TATOO_3 = "Tatouage 3";
inline const std::set<QString> ALL_EQUIP{EQUIP_HEAD,
                                  EQUIP_NECKLACE,
                                  EQUIP_CHEST,
                                  EQUIP_SHOES,
                                  EQUIP_ARM,
                                  EQUIP_RING_RIGHT,
                                  EQUIP_RING_LEFT,
                                  EQUIP_PANTS,
                                  EQUIP_NAME,
                                  EQUIP_RIGHT_WEAPON,
                                  EQUIP_LEFT_WEAPON,
                                  EQUIP_CATEGORY,
                                  EQUIP_RUNIQUE_TATOO_1,
                                  EQUIP_RUNIQUE_TATOO_2,
                                  EQUIP_RUNIQUE_TATOO_3};
inline const std::set<QString> ALL_EQUIP_ON_BODY{"",
                                          EQUIP_HEAD,
                                          EQUIP_NECKLACE,
                                          EQUIP_CHEST,
                                          EQUIP_SHOES,
                                          EQUIP_ARM,
                                          EQUIP_RING_RIGHT,
                                          EQUIP_RING_LEFT,
                                          EQUIP_PANTS,
                                          EQUIP_RIGHT_WEAPON,
                                          EQUIP_LEFT_WEAPON,
                                          EQUIP_RUNIQUE_TATOO_1,
                                          EQUIP_RUNIQUE_TATOO_2,
                                          EQUIP_RUNIQUE_TATOO_3};
const std::vector<QString> RAND_EQUIP_ON_BODY{
    EQUIP_HEAD,         EQUIP_NECKLACE,    EQUIP_CHEST,     EQUIP_SHOES,
    EQUIP_ARM,          EQUIP_RING_RIGHT,  EQUIP_RING_LEFT, EQUIP_PANTS,
    EQUIP_RIGHT_WEAPON, EQUIP_LEFT_WEAPON,
};
// Effect keys
inline const QString EFFECT_REINIT = "Reinit";
inline const QString EFFECT_NB_COOL_DOWN = "Tours de recharge";
// Effect to execute an atk with a decreasing success rate defined by a step
// on effect value
inline const QString EFFECT_NB_DECREASE_ON_TURN = "Decroissement pendant le tour";
// not used
inline const QString EFFECT_NB_DECREASE_BY_TURN = "Decroissement par tour";
// Effect to improve cuurent value of a stat by value
inline const QString EFFECT_VALUE_CHANGE = "Changement par valeur";
// Effect to improve cuurent value of a stat by percent
inline const QString EFFECT_PERCENT_CHANGE = "Changement par %";
inline const QString EFFECT_DELETE_BAD = "Supprime effet néfaste";
inline const QString EFFECT_INTO_DAMAGE = "% (stats) en dégâts";
inline const QString EFFECT_IMPROVE_HOTS = "Boost chaque HOT de .. %";
inline const QString EFFECT_BOOSTED_BY_HOTS =
    "Boost l'effet par nb HOTS presents en %";
inline const QString EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT = "Up/down degats en %";
inline const QString EFFECT_CHANGE_DAMAGES_RX_BY_PERCENT = "Up/down degats RX en %";
inline const QString EFFECT_CHANGE_HEAL_RX_BY_PERCENT = "Up/down heal RX en %";
inline const QString EFFECT_CHANGE_HEAL_TX_BY_PERCENT = "Up/down heal TX en %";
// Assess the nob of applies for a stat
inline const QString EFFECT_REPEAT_AS_MANY_AS = "Répète tant que possible";
// Effect to improve max value of a stat by percent (current value is updated by
// ratio)
inline const QString EFFECT_IMPROVE_BY_PERCENT_CHANGE = "Up par %";
// Effect to improve max value of a stat by value (current value is updated by
// ratio)
inline const QString EFFECT_IMPROVEMENT_STAT_BY_VALUE = "Up par valeur";
inline const QString EFFECT_NEXT_HEAL_IS_CRIT = "Prochaine attaque heal est crit";
inline const QString EFFECT_BUF_MULTI = "Buf multi";
inline const QString EFFECT_BLOCK_HEAL_ATK = "Bloque attaque de soin";
// Conditions on effect
// Atk is enable if damages are been tx during previous turn
inline const QString EFFECT_COND_DMG_PREV_TURN =
    "Active effet si Dégâts au tour précédent";
// Condition on atk
// Atk is enable if damages are been tx during previous turn
inline const QString CONDITION_DMG_PREV_TURN = "Dégâts au tour précédent";
inline const QString CONDITION_ENNEMIES_DIED = "Ennemis morts tours précédents";
inline const QString EFFECT_REPEAT_IF_HEAL =
    "Répète l'attaque(en % de chance) après heal tour prec.";
inline const QString EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL =
    "Buf par valeur d'autant de PV";
// Effect to modify the regen rate of stats such as vigor, mana, speed, hp,
// berseck by value.
inline const std::set<QString> EFFECTS{"",
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
                                EFFECT_COND_DMG_PREV_TURN};
inline const std::set<QString> ACTIVE_EFFECTS_ON_LAUNCH = {
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
    EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL};
inline const std::set<QString> EFFECTS_HOT_OR_DOT = {
    EFFECT_VALUE_CHANGE, EFFECT_REPEAT_AS_MANY_AS, EFFECT_PERCENT_CHANGE,
    EFFECT_NB_DECREASE_ON_TURN};
inline const QString EFFECT_ARRAY = "Effet";
inline const QString EFFECT_TYPE = "Type";
inline const QString EFFECT_VALUE = "Value";
inline const QString EFFECT_TARGET = "Cible";
inline const QString EFFECT_REACH = "Portée";
inline const QString EFFECT_STAT = "Stat";
inline const QString EFFECT_ACTIVE_TURNS = "Tours actifs";
inline const QString EFFECT_SUB_VALUE = "Valeur de l'effet";
inline const QString EFFECT_PASSIVE_TALENT = "Passif";
inline const QString EFFECT_COUNTER_TURN = "Compteur";
inline const QString EFFECT_IS_MAGIC = "Est Magique";
inline const QString EFFECT_LAUNCHER = "launcher";
inline const QString EFFECT_INDEX_TURN = "index tour";
// Form keys
inline const QString ENT_FORM = "Ent";
inline const QString BEAR_FORM = "Ours";
inline const QString STANDARD_FORM = "Standard";
inline const std::set<QString> ALL_FORMS = {STANDARD_FORM, ENT_FORM, BEAR_FORM};

// Class keys
inline const QString STANDARD_CLASS = "Standard";
inline const QString TANK_CLASS = "Tank";
inline const std::set<QString> ALL_CLASSES = {STANDARD_CLASS, TANK_CLASS};

class StatsType {
public:
  StatsType() = default; // Default constructor
  explicit StatsType(QString type) : m_Type(type) {}
  int m_CurrentValue = 0;
  int m_MaxValue = 0;
  int m_BaseEquipValue = 0;
  int m_RawMaxValue = 0;
  int m_CurrentRawValue = 0;
  QString m_Type;
  int m_BufEffectValue = 0;
  int m_BufEffectPercent = 0;
  int m_BufEquipValue = 0;
  int m_BufEquipPercent = 0;
  void InitValues(int current, int max) {
    m_CurrentValue = current;
    m_CurrentRawValue = current;
    m_MaxValue = max;
    m_RawMaxValue = max;
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
  StatsType m_RegenSpeed = StatsType(STATS_REGEN_SPEED);

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
      {STATS_REGEN_VIGOR, m_RegenVigor},
      {STATS_REGEN_SPEED, m_RegenSpeed}};
};

// IN GAME
const int NB_TURN_SUM_AGGRO = 5;

inline const std::set<QString> KERNEL_PLAYERS = {"Thalia", "Thrain", "Azrak Ombresang",
                                          "Elara la guerisseuse de la Lorien",
                                          "Test"};

// sounds
inline const QString CRITICAL_SOUND = "critical-hit.mp3";
inline const QString VICTORY_SOUND = "victory.mp3";
inline const QString PLAYER_DEATH_SOUND = "player-death-sound.mp3";
inline const QString GAME_OVER_SOUND = "game-over.mp3";
inline const QString POTION_SOUND = "potion.mp3";
inline const QString KILL_SOUND = "kill.mp3";

#endif // COMMON_H
