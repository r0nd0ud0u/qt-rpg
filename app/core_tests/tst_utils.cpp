#include <QDebug>
#include <QtTest>

#include "Application.h"
#include "playersmanager.h"
#include "utils.h"

//#include "tst_characters.cpp"

class utils_tests : public QObject {
  Q_OBJECT

private slots:
  void CompareByLevel_works();
};

void utils_tests::CompareByLevel_works() {
  // init
  AttaqueType atk1;
  atk1.level = 1;
  AttaqueType atk2;
  atk2.level = 1;
  // case same level
  bool result = Utils::CompareByLevel(atk1, atk2);
  Q_ASSERT(!result);
  // case level1 < level2
  atk2.level = 2;
  result = Utils::CompareByLevel(atk1, atk2);
  Q_ASSERT(result);
  // case level1 > level2
  atk1.level = 3;
  result = Utils::CompareByLevel(atk1, atk2);
  Q_ASSERT(!result);
}

class character_tests : public QObject {
  Q_OBJECT

private slots:
  void TestThalia_works();
};

void character_tests::TestThalia_works() {
  const QString thalia = "Thalia";
  const QString azrakRx = "Azrak Ombresang";
  const std::set<QString> activePlayers{thalia, azrakRx};
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  // add heroes
  pm->UpdateActivePlayers(activePlayers);
  auto *thaliaCh = pm->GetCharacterByName(thalia);
  auto *AzrakChRx = pm->GetCharacterByName(azrakRx);

  // attaque 1
  AttaqueType atk1 = thaliaCh->m_AtksByLevel[0];
  const auto &[conditionsOk, resultEffects, appliedEffects] =
      thaliaCh->ApplyAtkEffect(true, atk1, AzrakChRx, false);

  QCOMPARE(true, conditionsOk);
}

class player_manager_tests : public QObject {
  Q_OBJECT

public:
private slots:
  void GetDeadliestAlly_works();
  void ProcessDamageTXHealNeedyAlly_works();
  void LootNewEquipments_works();
};

void player_manager_tests::GetDeadliestAlly_works() {

  PlayersManager pl;
  Character *c1 = new Character;
  Character *c2 = new Character;

  // case pv c1 < pv c2
  c1->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue = 0;
  c1->m_Stats.m_AllStatsTable.at(STATS_HP).m_MaxValue = 1000;
  c2->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue = 1000;
  c2->m_Stats.m_AllStatsTable.at(STATS_HP).m_MaxValue = 1000;
  c1->m_Name = "c1";
  c2->m_Name = "c2";
  pl.m_HeroesList.push_back(c1);
  pl.m_HeroesList.push_back(c2);

  const auto output = pl.GetAllDeadliestAllies(characType::Hero);
  QCOMPARE("c1", output.value().front());

  // case pv c1 == pv c2
  c2->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue = 0;
  const auto output2 = pl.GetAllDeadliestAllies(characType::Hero).value();
  QCOMPARE("c1", output2.front());
  QCOMPARE("c2", output2.at(1));
}

void player_manager_tests::ProcessDamageTXHealNeedyAlly_works() {
  PlayersManager pl;
  Character *c1 = new Character;
  Character *c2 = new Character;

  // case pv c1 < pv c2
  c1->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue = 0;
  c1->m_Stats.m_AllStatsTable.at(STATS_HP).m_MaxValue = 1000;
  c2->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue = 1000;
  c2->m_Stats.m_AllStatsTable.at(STATS_HP).m_MaxValue = 1000;
  c1->m_Name = "c1";
  c2->m_Name = "c2";
  pl.m_HeroesList.push_back(c1);
  pl.m_HeroesList.push_back(c2);

  pl.ProcessDamageTXHealNeedyAlly(characType::Hero, 100);
  QCOMPARE(25, c1->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue);
}

void player_manager_tests::LootNewEquipments_works() {
  PlayersManager pl;
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  const auto result = pm->LootNewEquipments("Angmar");
  // rank if Angmar is 4 => 4 loots
  QCOMPARE(4, result.size());
  for (const auto &s : result) {
    QCOMPARE(s.m_Rank, s.m_StatsUpByLoot.size());
    for (const auto &stats : s.m_StatsUpByLoot) {
      if (s.m_Stats.m_AllStatsTable.count(stats) == 0) {
        continue;
      }
      auto &stuffStat = s.m_Stats.m_AllStatsTable.at(stats);
      const auto actual =
          (stuffStat.m_BufEquipPercent > 0 || stuffStat.m_BufEquipValue > 0);
      QCOMPARE(true, actual);
    }
  }
}

class game_manager_tests : public QObject {
  Q_OBJECT

public:
private slots:
  void ProcessOrderToPlay_works();
};

void game_manager_tests::ProcessOrderToPlay_works() {
  auto *gm = Application::GetInstance().m_GameManager.get();
  std::vector<QString> listPl;

  gm->m_PlayersManager->ClearHeroBossList();
  // init heroes
  Stats stats;
  // speed increasing by 10
  stats.m_AllStatsTable[STATS_SPEED].m_CurrentValue = 0;
  stats.m_AllStatsTable[STATS_HP].m_CurrentValue = 10;
  auto *hero1 = new Character("h1", characType::Hero, stats);
  stats.m_AllStatsTable[STATS_SPEED].m_CurrentValue += 10;
  stats.m_AllStatsTable[STATS_HP].m_CurrentValue = 10;
  auto *hero2 = new Character("h2", characType::Hero, stats);
  stats.m_AllStatsTable[STATS_SPEED].m_CurrentValue += 10;
  // no hp
  stats.m_AllStatsTable[STATS_HP].m_CurrentValue = 0;
  auto *hero3 = new Character("h3", characType::Hero, stats);
  // init boss
  stats.m_AllStatsTable[STATS_SPEED].m_CurrentValue += 10;
  auto *boss = new Character("b1", characType::Boss, stats);

  gm->m_PlayersManager->m_HeroesList.push_back(hero1);
  gm->m_PlayersManager->m_HeroesList.push_back(hero2);
  gm->m_PlayersManager->m_HeroesList.push_back(hero3);
  gm->m_PlayersManager->m_BossesList.push_back(boss);
  gm->ProcessOrderToPlay(listPl);
  QCOMPARE(4, listPl.size());
  QCOMPARE("h2", listPl[0]);
  QCOMPARE("h1", listPl[1]);
  QCOMPARE("h3", listPl[2]);
  QCOMPARE("b1", listPl[3]);
}

int main(int argc, char *argv[]) {
  auto app = std::make_unique<Application>(argc, argv);
  app->Init();

  utils_tests test;
  QTest::qExec(&test);

  player_manager_tests test2;
  QTest::qExec(&test2);

  character_tests test3;
  QTest::qExec(&test3);

  game_manager_tests test4;
  QTest::qExec(&test4);

  return 0;
}

#include "tst_utils.moc"
