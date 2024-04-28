#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "bossclass.h"
#include "utils.h"

#include "rust-rpg-bridge/attaque.h"
#include "rust-rpg-bridge/utils.h"

#include <fstream>

void PlayersManager::InitHeroes() {

  // Stats stats;
  // stats.m_AllStatsTable[STATS_HP].InitValues(135, 135);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(160, 160);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(40, 40);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(20, 20);
  // // TODO set max aggro 9999 is a good idea??
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 9999);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(12, 12);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(12, 12);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(7, 7);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(7, 7);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1);
  // const auto hero1 = new Character("Thalia", characType::Hero, stats);
  // hero1->m_Forms.push_back(ENT_FORM);
  // hero1->m_Forms.push_back(BEAR_FORM);

  // stats.m_AllStatsTable[STATS_HP].InitValues(145, 145);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(70, 70);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(130, 13);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(20, 20);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 9999);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(12, 12);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(6, 6);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(7, 7);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(7, 7);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1);
  // const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);
  // hero2->m_Forms.push_back(STANDARD_FORM);

  // stats.m_AllStatsTable[STATS_HP].InitValues(155, 155);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(20, 20);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(50, 50);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(50, 50);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(20, 20);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 9999);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(8, 8);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(7, 7);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(3, 3);
  // const auto hero3 = new Character("Thraïn", characType::Hero, stats);
  // hero3->m_Forms.push_back(STANDARD_FORM);

  // // character flo
  // stats.m_AllStatsTable[STATS_HP].InitValues(130, 130);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(180, 180);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 9999);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(8, 8);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(8, 8);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(7, 7);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1);
  // const auto hero4 = new Character("Elara la guerisseuse de la Lorien",
  //                                  characType::Hero, stats);
  // hero4->m_Forms.push_back(STANDARD_FORM);

  // // Lirion Vertefeuille
  // stats.m_AllStatsTable[STATS_HP].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(170, 170);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(30, 30);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 9999);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(15, 15);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(15, 15);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(12, 12);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(2, 2);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1);
  // const auto hero5 =
  //     new Character("Lirion Vertefeuille", characType::Hero, stats);
  // hero5->m_Forms.push_back(STANDARD_FORM);

  // // color
  // hero1->color = QColor("green");
  // hero2->color = QColor("orange");
  // hero3->color = QColor("blue");
  // hero4->color = QColor("pink");
  // hero5->color = QColor("brown");
  // hero1->m_ColorStr = "green";
  // hero2->m_ColorStr = "orange";
  // hero3->m_ColorStr = "blue";
  // hero4->m_ColorStr = "pink";
  // hero5->m_ColorStr = "brown";

  // std::set<Character *> heroes{hero4, hero1, hero2, hero3, hero5};
  // // TODO re order that list
  // m_AllHeroesList.push_back(hero4);
  // m_AllHeroesList.push_back(hero1);
  // m_AllHeroesList.push_back(hero2);
  // m_AllHeroesList.push_back(hero3);
  // m_AllHeroesList.push_back(hero5);

  // OutputCharactersInJson(m_AllHeroesList);

  std::for_each(m_AllHeroesList.begin(), m_AllHeroesList.end(), [&](Character *h) {
    h->LoadAtkJson();
    h->SortAtkByLevel();
    h->LoadStuffJson();
    std::unordered_map<QString, QString> table;
    for (auto &[bodypart, stuff] : h->m_WearingEquipment) {
      if (!bodypart.isEmpty()) {
        table[bodypart] = stuff.m_UniqueName;
      }
    }
    h->SetEquipment(table);
    h->ApplyEquipOnStats(m_AllEffectsOnGame[h->m_Name]);
  });

  for(const auto& h : m_AllHeroesList){
      if(h->m_Name == "Thalia"){
          const auto epParamTalent1 = h->LoadThaliaTalent();
          AddGameEffectOnAtk(h->m_Name, AttaqueType(), h->m_Name,
                             epParamTalent1, 0);
          h->ApplyEffeftOnStats(true, m_AllEffectsOnGame[h->m_Name]);
      }
      if(h->m_Name == "Azrak Ombresang"){
          const auto epParamTalent2 = h->LoadAzrakTalent();
          AddGameEffectOnAtk(h->m_Name, AttaqueType(), h->m_Name,
                             epParamTalent2, 0);
          h->ApplyEffeftOnStats(true, m_AllEffectsOnGame[h->m_Name]);
      }
      if(h->m_Name == "Thraïn"){
         const auto epParamTalent3 = h->LoadThrainTalent();
          AddGameEffectOnAtk(h->m_Name, AttaqueType(), h->m_Name,
                             epParamTalent3, 0);
         h->ApplyEffeftOnStats(true, m_AllEffectsOnGame[h->m_Name]);
      }
      if(h->m_Name == "Elara la guerisseuse de la Lorien"){
          const auto epParamTalent4 = h->LoadElaraTalent();
          // add passive powers
          h->m_Power.is_crit_heal_after_crit = true;
          h->m_Power.is_damage_tx_heal_needy_ally = true;
      }
  }

}

void PlayersManager::InitBosses() {
  // Stats stats;
  // stats.m_AllStatsTable[STATS_HP].InitValues(7000, 7000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(580, 580);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(603, 603);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(200, 200);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(21, 21);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(18, 18);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss1 =
  //     new Character("La bouche du Mordor", characType::Boss, stats);
  // boss1->color = QColor("red");
  // m_AllBossesList.push_back(boss1);
  // boss1->m_Forms.push_back(STANDARD_FORM);

  // stats.m_AllStatsTable[STATS_HP].InitValues(4000, 4000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(500, 500);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(500, 500);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(50, 50);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(50, 50);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);

  // for (int i = 0; i < 5; i++) {
  //   const auto boss2 =
  //       new Character(QString("Nazgul-%1").arg(i), characType::Boss, stats);
  //   boss2->m_Forms.push_back(STANDARD_FORM);
  //   boss2->color = QColor("red");
  //   m_AllBossesList.push_back(boss2);
  // }

  // stats.m_AllStatsTable[STATS_HP].InitValues(50000, 50000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(800, 800);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(180, 180);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(180, 180);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(28, 28);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(18, 18);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(14, 14);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss3 = new Character("Angmar", characType::Boss, stats);
  // boss3->color = QColor("red");
  // boss3->m_BossClass.m_Rank = 4;
  // m_AllBossesList.push_back(boss3);
  // boss3->m_Forms.push_back(STANDARD_FORM);

  // stats.m_AllStatsTable[STATS_HP].InitValues(20000, 20000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(800, 800);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(600, 600);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(250, 250);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(120, 120);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(20, 20);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(20, 20);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(20, 20);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss4 = new Character("Angmar le retour", characType::Boss, stats);
  // boss4->color = QColor("red");
  // m_AllBossesList.push_back(boss4);
  // boss4->m_BossClass.m_Rank = 4;
  // boss4->m_Forms.push_back(STANDARD_FORM);

  // // Angmar 10 PV
  // stats.m_AllStatsTable[STATS_HP].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(800, 800);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(180, 180);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(180, 180);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(28, 28);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(18, 18);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(14, 14);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss5 = new Character("Angmar10PV", characType::Boss, stats);
  // boss5->color = QColor("red");
  // boss5->m_BossClass.m_Rank = 4;
  // m_AllBossesList.push_back(boss5);
  // boss5->m_Forms.push_back(STANDARD_FORM);

  // // Carcaroth
  // stats.m_AllStatsTable[STATS_HP].InitValues(84000, 84000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 99999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss6 = new Character("Carcaroth", characType::Boss, stats);
  // boss6->color = QColor("red");
  // boss6->m_BossClass.m_Rank = 4;
  // m_AllBossesList.push_back(boss6);
  // boss6->m_Forms.push_back(STANDARD_FORM);

  // // beorn
  // stats.m_AllStatsTable[STATS_HP].InitValues(65000, 65000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(600, 600);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(750, 750);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(120, 120);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(150, 150);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(24, 24);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(18, 18);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss7 = new Character("Beorn", characType::Boss, stats);
  // boss7->color = QColor("red");
  // boss7->m_BossClass.m_Rank = 5;
  // m_AllBossesList.push_back(boss7);
  // boss7->m_Forms.push_back(STANDARD_FORM);

  // // miriel
  // stats.m_AllStatsTable[STATS_HP].InitValues(17000, 17000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(400, 400);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(50, 50);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(100, 100);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(10, 10);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(22, 22);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(12, 12);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss8 = new Character("Miriel", characType::Boss, stats);
  // boss8->color = QColor("red");
  // boss8->m_BossClass.m_Rank = 4;
  // m_AllBossesList.push_back(boss8);
  // boss8->m_Forms.push_back(STANDARD_FORM);

  // // ragnor

  // stats.m_AllStatsTable[STATS_HP].InitValues(45000, 45000);
  // stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(700, 700);
  // stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(9999, 9999);
  // stats.m_AllStatsTable[STATS_POW_PHY].InitValues(110, 110);
  // stats.m_AllStatsTable[STATS_POW_MAG].InitValues(40, 40);
  // stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_SPEED].InitValues(28, 28);
  // stats.m_AllStatsTable[STATS_CRIT].InitValues(16, 16);
  // stats.m_AllStatsTable[STATS_DODGE].InitValues(5, 5);
  // stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0);
  // stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0);
  // const auto boss9 = new Character("Ragnor", characType::Boss, stats);
  // boss9->color = QColor("red");
  // boss9->m_BossClass.m_Rank = 4;
  // m_AllBossesList.push_back(boss9);
  // boss9->m_Forms.push_back(STANDARD_FORM);

  // OutputCharactersInJson(m_AllBossesList);
  for (const auto &boss : m_AllBossesList) {
    boss->m_ColorStr = "red";
    boss->LoadAtkJson();
    boss->SortAtkByLevel();
    boss->LoadStuffJson();
    boss->ApplyEquipOnStats(m_AllEffectsOnGame[boss->m_Name]);
  }
}

bool PlayersManager::UpdateActivePlayers(const std::set<QString> &playersList) {
  if (playersList.empty()) {
    return false;
  }
  std::for_each(m_AllHeroesList.begin(), m_AllHeroesList.end(),
                [&](Character *c) {
                  if (c != nullptr && playersList.count(c->m_Name)) {
                    m_HeroesList.push_back(c);
                  }
                });
  if (!m_HeroesList.empty()) {
    m_SelectedHero = m_HeroesList.front();
  }

  std::for_each(m_AllBossesList.begin(), m_AllBossesList.end(),
                [&](Character *c) {
                  if (c != nullptr && playersList.count(c->m_Name)) {
                    m_BossesList.push_back(c);
                  }
                });
  return (m_HeroesList.size() > 0 && m_BossesList.size() > 0);
}

void PlayersManager::LoadAllEquipmentsJson() {
  // List all equipment
  QString directoryPath =
      OFFLINE_ROOT_EQUIPMENT; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  // Detect all directories, each directory is a part of the body
  QStringList subdirectories =
      directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (const auto &subdirPath : subdirectories) {
    QDir subdir(directoryPath + subdirPath);
    if (!subdir.exists()) {
      qDebug() << "Directory does not exist: " << subdirPath;
    }
    QStringList fileList = subdir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QString &file : fileList) {
      QFile json(directoryPath + subdirPath + "/" + file);
      if (!json.open(QFile::ReadOnly | QFile::Text)) {
        Application::GetInstance().log(" Could not open the file for reading " +
                                       directoryPath + "/" + file);
        return;
      } else {
        // Convert json file to QString
        QTextStream out(&json);
#if QT_VERSION_MAJOR == 6
        out.setEncoding(QStringConverter::Encoding::Utf8);
#else
        out.setCodec("UTF-8");
#endif
        QString msg = out.readAll();
        json.close();
        const auto jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
        // decode json

        Stuff stuff;
        stuff.m_Name = jsonDoc[EQUIP_NAME].toString();
        stuff.m_UniqueName = jsonDoc[EQUIP_UNIQUE_NAME].toString();

        for (const auto &stats : ALL_STATS) {
          if (stuff.m_Stats.m_AllStatsTable.count(stats) == 0) {
            continue;
          }
          // init
          auto &stuffStat = stuff.m_Stats.m_AllStatsTable[stats];
          stuffStat.InitValues(0, 0);
          QJsonArray jsonArray = jsonDoc[stats].toArray();
          for (const auto &elem : jsonArray) {
            if (elem.isObject()) {
              const QJsonObject item = elem.toObject();
              for (const auto &key : item.keys()) {
                const auto &val = item[key];
                if (val.isDouble()) {
                  if (key == "percent") {
                    stuffStat.m_BufEquipPercent =
                        static_cast<int>(val.toDouble());
                  } else if (key == "value") {
                    stuffStat.m_BufEquipValue =
                        static_cast<int>(val.toDouble());
                  }
                }
              }
            }
          }
        }
        const auto name =
            (stuff.m_UniqueName.isEmpty()) ? stuff.m_Name : stuff.m_UniqueName;
        m_Equipments[jsonDoc[EQUIP_CATEGORY].toString()][name] = stuff;
      }
    }
  }
}

Character *PlayersManager::GetCharacterByName(const QString &name) {
  for (auto *boss : m_BossesList) {
    if (name == boss->m_Name) {
      return boss;
    }
  }
  for (auto *hero : m_HeroesList) {
    if (name == hero->m_Name) {
      return hero;
    }
  }

  return nullptr;
}

void PlayersManager::AddGameEffectOnAtk(const QString &launcherName,
                                        const AttaqueType &atk,
                                        const QString &targetName,
                                        const std::vector<effectParam> &effects,
                                        const int currentTurn) {
  for (const auto &e : effects) {
    GameAtkEffects gae;
    gae.launcher = launcherName;
    gae.atk = atk;
    gae.allAtkEffects = e;
    gae.launchingTurn = currentTurn;
    m_AllEffectsOnGame[targetName].push_back(gae);
  }
}

QStringList
PlayersManager::RemoveTerminatedEffectsOnPlayer(const QString &curPlayerName) {
  QStringList sl;
  if (m_AllEffectsOnGame.count(curPlayerName) == 0) {
    return sl;
  }
  auto &gaeTable = m_AllEffectsOnGame[curPlayerName];
  for (auto it = gaeTable.begin(); it != gaeTable.end(); it++) {
    if (it->allAtkEffects.counterTurn == it->allAtkEffects.nbTurns) {
      const auto effectName =
          build_effect_name(it->allAtkEffects.effect.toStdString(),
                            it->allAtkEffects.statsName.toStdString(), true)
              .data();
      const auto terminated = QString("L'effet %1 sur %2 est terminé.(%3)")
                                  .arg(effectName)
                                  .arg(curPlayerName)
                                  .arg(it->atk.name);
      sl.push_back(terminated);
      // remove malus effect from player
      auto *player = GetCharacterByName(curPlayerName);
      if (player != nullptr) {
        player->RemoveMalusEffect(it->allAtkEffects);
      }
    }
  }
  auto newEnd = std::remove_if(
      gaeTable.begin(), gaeTable.end(), [](const GameAtkEffects &element) {
        return element.allAtkEffects.nbTurns ==
               element.allAtkEffects
                   .counterTurn; // remove elements where this is true
      });
  gaeTable.erase(newEnd, gaeTable.end());
  return sl;
}

QStringList PlayersManager::ApplyEffectsOnPlayer(const QString &curPlayerName,
                                                 const int currentTurn,
                                                 const bool fromLaunch) {
  QStringList logs;
  if (m_AllEffectsOnGame.count(curPlayerName) == 0) {
    return logs;
  }
  auto &gaeTable = m_AllEffectsOnGame[curPlayerName];
  auto *targetPl = GetCharacterByName(curPlayerName);

  QStringList localLog;

  if (targetPl != nullptr) {
    int hotAndDot = 0;
    for (auto &gae : gaeTable) {
      if (gae.launchingTurn == currentTurn) {
        // effect is applicable at launch of one character and then at the next
        // turn of the target
        continue;
      }
      if (gae.allAtkEffects.statsName == STATS_HP &&
          EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0) {
        // sum of hot and dot
        hotAndDot += gae.allAtkEffects.value;
        QString type = "DOT->";
        if (gae.allAtkEffects.value > 0) {
          type = "HOT->";
        }
        localLog.append(QString("%1 valeur: %2, atk: %3")
                            .arg(type)
                            .arg(gae.allAtkEffects.value)
                            .arg(gae.atk.name));
      } else if (auto *launcherPl = GetCharacterByName(gae.launcher);
                 launcherPl != nullptr) {
        const auto [output, _] = launcherPl->ApplyOneEffect(
            targetPl, gae.allAtkEffects, fromLaunch, gae.atk);
        if (!output.isEmpty()) {
          localLog.append(output);
        }
      }
    }
    auto &localStat = targetPl->m_Stats.m_AllStatsTable[STATS_HP];
    localStat.m_CurrentValue += hotAndDot;
    if (hotAndDot != 0) {
      localLog.append(QString("HOT et DOT totaux: %1").arg(hotAndDot));
      // current value must be included between 0 and max value
      localStat.m_CurrentValue =
          std::min(localStat.m_CurrentValue, localStat.m_MaxValue);
      localStat.m_CurrentValue = std::max(localStat.m_CurrentValue, 0);
    }
  }
  if (!localLog.isEmpty()) {
    logs.append(localLog.join("\n"));
  }

  return logs;
}

void PlayersManager::ApplyRegenStats(const characType &type) {
  std::vector<Character *> playerList = m_HeroesList;
  if (type == characType::Hero) {
    playerList = m_HeroesList;
  } else if (type == characType::Boss) {
    playerList = m_BossesList;
  }
  for (const auto &pl : playerList) {

    auto &hp = pl->m_Stats.m_AllStatsTable[STATS_HP];
    const auto &regenHp = pl->m_Stats.m_AllStatsTable[STATS_REGEN_HP];
    auto &mana = pl->m_Stats.m_AllStatsTable[STATS_MANA];
    const auto &regenMana = pl->m_Stats.m_AllStatsTable[STATS_REGEN_MANA];
    auto &berseck = pl->m_Stats.m_AllStatsTable[STATS_BERSECK];
    const auto &regenBerseck = pl->m_Stats.m_AllStatsTable[STATS_RATE_BERSECK];
    auto &vigor = pl->m_Stats.m_AllStatsTable[STATS_VIGOR];
    const auto &regenVigor = pl->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR];
    auto &speed = pl->m_Stats.m_AllStatsTable[STATS_SPEED];
    const auto &regenSpeed = pl->m_Stats.m_AllStatsTable[STATS_REGEN_SPEED];

    hp.m_CurrentValue = std::min(
        hp.m_MaxValue,
        hp.m_CurrentValue + hp.m_MaxValue * regenHp.m_CurrentValue / 100);
    mana.m_CurrentValue = std::min(
        mana.m_MaxValue,
        mana.m_CurrentValue + mana.m_MaxValue * regenMana.m_CurrentValue / 100);
    vigor.m_CurrentValue =
        std::min(vigor.m_MaxValue,
                 vigor.m_CurrentValue +
                     vigor.m_MaxValue * regenVigor.m_CurrentValue / 100);
    berseck.m_CurrentValue = std::min(
        berseck.m_MaxValue, berseck.m_CurrentValue + regenBerseck.m_CurrentValue);
    // Speed
    speed.m_CurrentValue += regenSpeed.m_CurrentValue;
    speed.m_MaxValue += regenSpeed.m_CurrentValue;
    speed.m_RawMaxValue += regenSpeed.m_CurrentValue;
  }
}

QString PlayersManager::FormatAtkOnEnnemy(const int damage) {
  return QString("fait %1 de dégâts!").arg(damage);
}

QString PlayersManager::FormatAtkOnAlly(const int damage) {
  return QString("soigne de %4 PV!").arg(damage);
}

QString PlayersManager::FormatAtk(const QString player2,
                                  const QString &atkName) {
  return QString("utilise %2 sur %3!").arg(atkName).arg(player2);
}

int PlayersManager::GetNbOfStatsInEffectList(const Character *chara,
                                             const QString &statsName) const {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return 0;
  }

  int counter = 0;
  for (const auto &e : m_AllEffectsOnGame.at(chara->m_Name)) {
    if (e.allAtkEffects.statsName == statsName) {
      counter++;
    }
  }
  return counter;
}

void PlayersManager::ResetCounterOnOneStatsEffect(const Character *chara,
                                                  const QString &statsName) {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return;
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (e.allAtkEffects.statsName == statsName) {
      e.allAtkEffects.counterTurn = 0;
    }
  }
}

QString PlayersManager::DeleteOneBadEffect(const Character *chara) {
  if (chara == nullptr) {
    // debug
    return "No target encountered on DeleteOneBadEffect";
  }
  if (m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    // output channel log
    return "Aucun effet actif.\n";
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name])
    // TODO rule about debuf
    // a DOT can be a debuf for example
    if (e.allAtkEffects.target == TARGET_ENNEMY &&
        !e.allAtkEffects.statsName.isEmpty()) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
      return "supprime un effet néfaste.\n";
    }

  return "Pas de mauvais effet supprimé.\n";
}

QString PlayersManager::DeleteAllBadEffect(const Character *chara) {
  if (chara == nullptr) {
    // debug
    return "No target encountered on DeleteAllBadEffect";
  }
  if (m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    // output channel log
    return "Aucun effet actif.\n";
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (e.allAtkEffects.value < 0) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
    }
  }
  return "supprime tous les effets néfastes.\n";
}

void PlayersManager::ImproveHotsOnPlayers(const int valuePercent,
                                          const characType launcherType) {
  std::vector<Character *> playerList;

  if (launcherType == characType::Hero) {
    playerList = m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = m_BossesList;
  }
  for (const auto &pl : playerList) {
    if (pl == nullptr || m_AllEffectsOnGame.count(pl->m_Name) == 0) {
      continue;
    }
    for (auto &e : m_AllEffectsOnGame[pl->m_Name]) {
      if (e.allAtkEffects.statsName == STATS_HP &&
          ALLIES_TARGETS.count(e.allAtkEffects.target) > 0) {
        e.allAtkEffects.value += e.allAtkEffects.value * valuePercent / 100;
      }
    }
  }
}

void PlayersManager::IncrementCounterEffect() {
  for (auto &[playerName, gaeTable] : m_AllEffectsOnGame) {
    for (auto &gae : gaeTable) {
      gae.allAtkEffects.counterTurn++;
    }
  }
}

/// Boss is deleted from list
/// Hero has only his life staying at 0.
/// That method returns a QStringlist with the different died played.
/// That list will be displayed on channel logs.
QStringList PlayersManager::CheckDiedPlayers(const characType &launcherType) {
  std::vector<Character *> *playerList = nullptr;

  if (launcherType == characType::Hero) {
    playerList = &m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = &m_BossesList;
  }
  if (playerList == nullptr) {
    return QStringList();
  }

  if (launcherType == characType::Hero) {
    return QStringList();
  }

  QStringList output;

  const auto newEnd = std::remove_if(
      playerList->begin(), playerList->end(), [&output](const Character *pl) {
        if (pl == nullptr) {
          return false;
        }
        const auto &hp = pl->m_Stats.m_AllStatsTable.at(STATS_HP);
        QString name = pl->m_Name;
        const bool isDead = hp.m_CurrentValue <= 0;
        if (isDead) {
          output.append(pl->m_Name);
        }
        return isDead; // remove elements where this is true
      });

  // Delete only boss player
  if (launcherType == characType::Boss) {
    // TODO check if bosses has another phase
    // if yes init that phase
    // if not erase the boss of the list
    playerList->erase(newEnd, playerList->end());
  }

  return output;
}

void PlayersManager::AddSupAtkTurn(
    const characType &launcherType,
    std::vector<QString> &playerOrderTable) const {
  std::vector<Character *> playerList1;
  std::vector<Character *> playerList2;

  if (launcherType == characType::Hero) {
    playerList1 = m_HeroesList;
    playerList2 = m_BossesList;
  } else if (launcherType == characType::Boss) {
    playerList2 = m_HeroesList;
    playerList1 = m_BossesList;
  }

  const int speedThreshold = 100;
  for (auto *pl1 : playerList1) {
    auto &speedPl1 =
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
    for (const auto &pl2 : playerList2) {
      const auto &speedpl2 =
          pl2->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
      if (speedPl1 - speedpl2 >= speedThreshold) {
        // Update of current value and max value
        speedPl1 -= speedThreshold;
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_MaxValue -=
            speedThreshold;
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_RawMaxValue -=
            speedThreshold;
        playerOrderTable.push_back(pl1->m_Name);
        break;
      }
    }
  }
}

std::tuple<bool, QString, QStringList>
PlayersManager::IsDodging(const std::vector<TargetInfo *> &targetList,
                          const AttaqueType &atk) {
  QString plName;
  QStringList output;
  const bool isDodging =
      std::any_of(targetList.begin(), targetList.end(),
                  [this, &plName, &output, &atk](const TargetInfo *ti) {
                    if (ti == nullptr) {
                      return false;
                    }
                    if (ti->get_is_targeted()) {
                      const auto *targetChara =
                          this->GetCharacterByName(ti->get_name().data());
                      plName = ti->get_name().data();
                      const auto [charIsDodging, randNbStr] =
                          targetChara->IsDodging(atk);
                      output.append(randNbStr);
                      return charIsDodging;
                    }
                    return false;
                  });

  return std::make_tuple(isDodging, plName, output);
}

void PlayersManager::AddExpForHeroes(const int exp) {
  for (auto &pl : m_HeroesList) {
    pl->AddExp(exp);
  }
}

/**
 * @brief PlayersManager::GetAllDeadliestAllies
 * @param launcherType : enable the check on list : boss or hero
 * @return optional<std::vector<QString>> : if any, the list of the names of
 * characters with minimal HP
 */
std::optional<std::vector<QString>>
PlayersManager::GetAllDeadliestAllies(const characType &launcherType) const {
  const std::vector<Character *> *playerList = nullptr;

  if (launcherType == characType::Hero) {
    playerList = &m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = &m_BossesList;
  }
  if (playerList == nullptr) {
    return std::nullopt;
  }

  std::vector<QString> output;

  const auto minElement = std::min_element(
      playerList->begin(), playerList->end(),
      [](const Character *char1, const Character *char2) {
        if (char1 == nullptr || char2 == nullptr) {
          return false;
        }
        const auto &stat1 = char1->m_Stats.m_AllStatsTable.at(STATS_HP);
        const auto &stat2 = char2->m_Stats.m_AllStatsTable.at(STATS_HP);

        // ratio
        const double ratio1 = (stat1.m_MaxValue > 0)
                                  ? static_cast<double>(stat1.m_CurrentValue) /
                                        static_cast<double>(stat1.m_MaxValue)
                                  : 1;
        const double ratio2 = (stat2.m_MaxValue > 0)
                                  ? static_cast<double>(stat2.m_CurrentValue) /
                                        static_cast<double>(stat2.m_MaxValue)
                                  : 1;

        return ratio1 < ratio2;
      });

  const auto &chp =
      (*minElement)->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue;

  std::for_each(
      playerList->begin(), playerList->end(), [&](const Character *c) {
        const auto &hp = c->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue;
        if (hp == chp) {
          output.push_back(c->m_Name);
        }
      });

  return !output.empty() ? std::optional(output) : std::nullopt;
}

/**
 * @brief PlayersManager::ProcessDamageTXHealNeedyAlly
 * Assess all the allies with the minimal hp ratio in a std::vector
 * Choose randomly one in this list and increase its hp current value by 25% of
 * damageTX
 * @param launcherType
 * @param damageTX
 * @return QString: not used at the moment, should be used for the output on
 * channel log
 */
QString
PlayersManager::ProcessDamageTXHealNeedyAlly(const characType &launcherType,
                                             const int damageTX) {

  if (damageTX == 0) {
    return "";
  }

  const auto alliesStr =
      GetAllDeadliestAllies(launcherType).value_or(std::vector<QString>{});
  if (alliesStr.empty()) {
    return "";
  }
  const auto randNb = get_random_nb(0, alliesStr.size() - 1);
  auto *c = GetCharacterByName(alliesStr[randNb]);
  if (c == nullptr) {
    return "";
  }
  auto &hp = c->m_Stats.m_AllStatsTable.at(STATS_HP);
  hp.m_CurrentValue = std::min(
      hp.m_MaxValue, hp.m_CurrentValue + static_cast<int>(0.25 * damageTX));

  return "";
}

/**
 * @brief PlayersManager::GetPlayerListByType
 * There is a number n of defined characType. => n list of characters
 * That method returns a pointer to the list according the characType.
 * nullptr if nothing has been found
 * @return a pointer std::vector<Character *> *
 */
std::vector<Character *> *
PlayersManager::GetPlayerListByType(const characType &launcherType) {
  std::vector<Character *> *playerList = nullptr;

  if (launcherType == characType::Hero) {
    playerList = &m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = &m_BossesList;
  }

  return playerList;
}

/**
 * @brief PlayersManager::ProcessIsRandomTarget
 * For each character in the game, the boolean is_random_target is set to false.
 * In case of atk with effect containing REACH_RAND_INDIVIDUAL,
 * we have to process the one of should be chosen.
 */
void PlayersManager::ProcessIsRandomTarget() const {
  // Reset
  std::for_each(m_HeroesList.begin(), m_HeroesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_random_target(false);
                  }
                });
  std::for_each(m_BossesList.begin(), m_BossesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_random_target(false);
                  }
                });

  // get random numbers
  const int64_t randHeroNb = get_random_nb(0, m_HeroesList.size() - 1);
  const int64_t randBossNb = get_random_nb(0, m_BossesList.size() - 1);

  // set who is the random reach
  auto *hero = m_HeroesList[randHeroNb];
  auto *boss = m_BossesList[randBossNb];
  if (hero != nullptr && hero->m_ExtCharacter != nullptr) {
    hero->m_ExtCharacter->set_is_random_target(true);
  }
  if (boss != nullptr && boss->m_ExtCharacter != nullptr) {
    boss->m_ExtCharacter->set_is_random_target(true);
  }
}

/**
 * @brief PlayersManager::ResetIsFirstRound
 * The boolean is_first_round is reset for all the characters of the game.
 */
void PlayersManager::ResetIsFirstRound() const {
  // Reset
  std::for_each(m_HeroesList.begin(), m_HeroesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_first_round(true);
                  }
                });
  std::for_each(m_BossesList.begin(), m_BossesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_first_round(true);
                  }
                });
}

std::vector<Stuff> PlayersManager::LootNewEquipments(const QString &name) {
  uint64_t rank = 0;
  int stuffClass = 0;
  std::vector<Stuff> newStuffs;
  for (const auto *boss : m_AllBossesList) {
    if (boss != nullptr && name == boss->m_Name) {
      rank = boss->m_BossClass.m_Rank;
      break;
    }
  }
  const auto nbOfLoots = rank;
  const auto probaLoot =
      (rank < static_cast<uint64_t>(BossClass::PROBA_LOOTS.size()))
          ? BossClass::PROBA_LOOTS.at(rank)
          : std::vector<uint64_t>{};

  if (nbOfLoots == 0) {
    return newStuffs;
  }
  for (int i = 0; i < nbOfLoots; i++) {
    const auto randProba = static_cast<uint64_t>(get_random_nb(0, 100));
    // Assess the rank of the loot
    for (int j = 0; j < probaLoot.size() - 1; j++) {
      if (randProba >= probaLoot[j] && randProba < probaLoot[j + 1]) {
        stuffClass = j + 1;
        break;
      }
    }
    // no loot found
    if (stuffClass == 0) {
      continue;
    }

    // Create stuff
    Stuff stuff;
    // add name
    const auto randEquipType = get_random_nb(0, RAND_EQUIP_ON_BODY.size() - 1);
    const auto &equipType = RAND_EQUIP_ON_BODY.at(randEquipType);
    const auto indexEquipName =
        get_random_nb(0, m_RandomEquipName[equipType].size() - 1);
    stuff.m_UniqueName =
        QString("%1-%2-%3")
            .arg(m_RandomEquipName[equipType].at(indexEquipName))
            .arg(stuffClass)
            .arg(Utils::getCurrentTimeAsString());
    stuff.m_Name = m_RandomEquipName[equipType].at(indexEquipName);
    // add body part
    stuff.m_BodyPart = equipType;
    // add class
    stuff.m_Rank = stuffClass;
    // stuffClass == nb of effects of the loot
    const auto nbOfEffets = stuffClass;

    for (int k = 0; k < nbOfEffets; k++) {
      const auto index = get_random_nb(0, BossClass::BONUS_STAT_STR.size() - 1);
      const auto &stat = BossClass::BONUS_STAT_STR.at(index);
      const auto *bonus = BossClass::BONUS_LIST.at(stat).at(stuffClass - 1);
      if (bonus->get_is_percent()) {
        stuff.m_Stats.m_AllStatsTable[stat].m_BufEquipPercent =
            bonus->get_value();
      } else {
        stuff.m_Stats.m_AllStatsTable[stat].m_BufEquipValue =
            bonus->get_value();
      }
    }
    // armur bonus does not contain 'no loot'(0) whereas stuffClass is starting
    // at 'no loot'(0)
    const auto armurBonus = (stuffClass - 1 < BossClass::ARMOR.size())
                                ? BossClass::ARMOR[stuffClass - 1]
                                : BossClass::ARMOR[BossClass::ARMOR.size() - 1];
    stuff.m_Stats.m_AllStatsTable[STATS_ARM_MAG].m_BufEquipValue += armurBonus;
    stuff.m_Stats.m_AllStatsTable[STATS_ARM_PHY].m_BufEquipValue += armurBonus;

    newStuffs.push_back(stuff);
  }

  return newStuffs;
}

void PlayersManager::InitRandomEquip() {
  for (const auto &equip : RAND_EQUIP_ON_BODY) {
    // Create an object of ifstream (input file stream) class
    std::ifstream inputFile;
    // Open a file named "example.txt" for reading
    const QString path = OFFLINE_RAND_NAME_STUFF + equip + ".txt";
    inputFile.open(path.toStdString());

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
      return; // Return an error code
    }

    std::string line;
    while (std::getline(inputFile, line)) {
      line = line.substr(2);
      m_RandomEquipName[equip].push_back(QString::fromStdString(line));
    }
    // Close the file when done
    inputFile.close();
  }
}

/**
 * @brief PlayersManager::GetHeroMostAggro
 * Returns the name of the hero character with the most aggro and the value of
 * the aggro In case of empty list, returns nullopt In case of equal aggro, the
 * output hero is randomly picked among the aggro-equal ones.
 */
std::optional<std::pair<QString, int>>
PlayersManager::GetHeroMostAggro() const {
  std::vector<const Character *> mostAggroCh;
  if (m_HeroesList.empty()) {
    return std::nullopt;
  }
  std::for_each(
      m_HeroesList.begin(), m_HeroesList.end(), [&](const Character *c) {
        if (!mostAggroCh.empty()) {
          const auto &aggroStat1 = mostAggroCh.front()
                                       ->m_Stats.m_AllStatsTable.at(STATS_AGGRO)
                                       .m_CurrentValue;
          const auto &aggroStat2 =
              c->m_Stats.m_AllStatsTable.at(STATS_AGGRO).m_CurrentValue;
          if (aggroStat1 < aggroStat2) {
            mostAggroCh.clear();
            mostAggroCh.push_back(c);
          } else if (aggroStat1 == aggroStat2) {
            mostAggroCh.push_back(c);
          }
        } else {
          mostAggroCh.push_back(c);
        }
      });
  const auto randNb = get_random_nb(0, mostAggroCh.size() - 1);

  return std::make_pair(mostAggroCh[randNb]->m_Name,
                        mostAggroCh[randNb]
                            ->m_Stats.m_AllStatsTable.at(STATS_AGGRO)
                            .m_CurrentValue);
}

void PlayersManager::OutputCharactersInJson(
    const std::vector<Character *> l) const {
  for (const auto *h : l) {
    if (h == nullptr) {
      continue;
    }
    QFile file;
    QDir logDir;
    QString path = OFFLINE_CHARACTERS;
    logDir.mkpath(path);
    // init json doc
    QJsonObject obj;

    obj.insert(CH_NAME, h->m_Name);
    const auto type =
        (h->m_type == characType::Boss) ? CH_TYPE_BOSS : CH_TYPE_HERO;
    obj.insert(CH_TYPE, type);
    obj.insert(CH_LEVEL, h->m_Level);
    obj.insert(CH_COLOR, h->m_ColorStr);
    obj.insert(CH_RANK, h->m_BossClass.m_Rank);

    for (const auto &stats : ALL_STATS) {
      if (h->m_Stats.m_AllStatsTable.count(stats) == 0) {
        continue;
      }
      const auto &st = h->m_Stats.m_AllStatsTable.at(stats);
      QJsonObject item;
      QJsonArray jsonArray;
      item[CH_CURRENT_VALUE] = st.m_CurrentValue;
      item[CH_MAX_VALUE] = st.m_MaxValue;
      jsonArray.append(item);
      if (!jsonArray.empty()) {
        obj[stats] = jsonArray;
      }
    }

    // output json
    QJsonDocument doc(obj);
    QString logFilePath = logDir.filePath(path + h->m_Name + ".json");
    file.setFileName(logFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      Application::GetInstance().log(" Could not open the file for writing " +
                                     logFilePath);
    }
    QTextStream out(&file);
#if QT_VERSION_MAJOR == 6
    out.setEncoding(QStringConverter::Encoding::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out << doc.toJson() << "\n";
  }
}

void PlayersManager::LoadAllCharactersJson() {
  // List all equipment
  QString directoryPath = OFFLINE_CHARACTERS; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  QStringList fileList =
      directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
  for (const QString &file : fileList) {
    QFile json(directoryPath + file);
    if (!json.open(QFile::ReadOnly | QFile::Text)) {
      Application::GetInstance().log(" Could not open the file for reading " +
                                     directoryPath + "/" + file);
      return;
    } else {
      // Convert json file to QString
      QTextStream out(&json);
#if QT_VERSION_MAJOR == 6
      out.setEncoding(QStringConverter::Encoding::Utf8);
#else
      out.setCodec("UTF-8");
#endif
      QString msg = out.readAll();
      json.close();
      const auto jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
      // decode json

      Character *c = new Character("", characType::Hero, {});
      c->m_Name = jsonDoc[CH_NAME].toString();
      c->m_type = (jsonDoc[CH_TYPE].toString() == CH_TYPE_BOSS)
                      ? characType::Boss
                      : characType::Hero;
      c->m_Level = jsonDoc[CH_LEVEL].toDouble();
      c->color = QColor(jsonDoc[CH_COLOR].toString());
      c->m_ColorStr = jsonDoc[CH_COLOR].toString();
      c->m_BossClass.m_Rank = jsonDoc[CH_RANK].toDouble();

      for (const auto &stats : ALL_STATS) {
        if (c->m_Stats.m_AllStatsTable.count(stats) == 0) {
          continue;
        }

        // init
        QJsonArray jsonArray = jsonDoc[stats].toArray();
        for (const auto &elem : jsonArray) {
          if (elem.isObject()) {
            const QJsonObject item = elem.toObject();
            const auto current =
                static_cast<int>(item[CH_CURRENT_VALUE].toDouble());
            const auto max = static_cast<int>(item[CH_MAX_VALUE].toDouble());
            c->m_Stats.m_AllStatsTable[stats].InitValues(current, max);
          }
        }
      }
      if (c->m_type == characType::Hero) {
        m_AllHeroesList.push_back(c);
      } else {
        m_AllBossesList.push_back(c);
      }
    }
  }
}
