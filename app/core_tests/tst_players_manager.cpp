#include <QDebug>
#include <QtTest>

#include "Application.h"
#include "playersmanager.h"

#include "classes/attak.cpp"
#include "classes/character_fixtures.cpp"

class player_manager_tests : public QObject {
  Q_OBJECT

public:
private slots:
  void GetDeadliestAlly_works();
  void ProcessDamageTXHealNeedyAlly_works();
  void LootNewEquipments_works();
  void CheckDiedPlayers_works();
  void GetNbOfActiveHotsOnHeroes_works();
  void RemoveTerminatedEffectsOnPlayer_works();
  void ResetAllEffectsOnPlayer_works();
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

void player_manager_tests::CheckDiedPlayers_works() {
  auto *gm = Application::GetInstance().m_GameManager.get();
  gm->m_PlayersManager->ClearHeroBossList();

  auto result = gm->m_PlayersManager->CheckDiedPlayers(characType::Boss);
  QVERIFY(result.isEmpty());
  result = gm->m_PlayersManager->CheckDiedPlayers(characType::Hero);
  QVERIFY(result.isEmpty());
  Stats stats;
  gm->m_PlayersManager->m_HeroesList.push_back(
      new Character("c1", characType::Hero, stats));
  result = gm->m_PlayersManager->CheckDiedPlayers(characType::Hero);
  QCOMPARE(1, result.size());
  QCOMPARE("c1", result.front());
  QCOMPARE(1, gm->m_PlayersManager->m_HeroesList.size());

  gm->m_PlayersManager->m_BossesList.push_back(
      new Character("b1", characType::Boss, stats));
  result = gm->m_PlayersManager->CheckDiedPlayers(characType::Boss);
  QCOMPARE(1, result.size());
  QCOMPARE("b1", result.front());
  QCOMPARE(0, gm->m_PlayersManager->m_BossesList.size());
}

void player_manager_tests::GetNbOfActiveHotsOnHeroes_works() {
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  pm->m_AllEffectsOnGame.clear();
  auto result = pm->GetNbOfActiveHotsOnHeroes();
  QCOMPARE(0, result);

  AttaqueType atk1 = SimpleHot();
  pm->AddGameEffectOnAtk("Test", atk1, "Test", atk1.m_AllEffects, 1);
  result = pm->GetNbOfActiveHotsOnHeroes();
  QCOMPARE(1, result);
}

void player_manager_tests::RemoveTerminatedEffectsOnPlayer_works() {
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  pm->m_AllEffectsOnGame.clear();
  QCOMPARE(0, pm->m_AllEffectsOnGame["Test"].size());

  pm->RemoveTerminatedEffectsOnPlayer("Test");
  QCOMPARE(0, pm->m_AllEffectsOnGame["Test"].size());

  AttaqueType atk1 = SimpleHot();
  pm->AddGameEffectOnAtk("Test", atk1, "Test", atk1.m_AllEffects, 1);

  AttaqueType atk2 = SimpleHot();
  atk2.m_AllEffects.front().counterTurn = atk2.m_AllEffects.front().nbTurns;
  pm->AddGameEffectOnAtk("Test", atk2, "Test", atk2.m_AllEffects, 1);
  pm->RemoveTerminatedEffectsOnPlayer("Test");
  QCOMPARE(1, pm->m_AllEffectsOnGame["Test"].size());
}

void player_manager_tests::ResetAllEffectsOnPlayer_works(){
    auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
    auto testCh = GetTestCharacter();
    pm->m_AllEffectsOnGame.clear();
    pm->ResetAllEffectsOnPlayer(&testCh);

    QCOMPARE(0, pm->m_AllEffectsOnGame["Test"].size());

    AttaqueType atk1 = SimpleHot();
    pm->AddGameEffectOnAtk("Test", atk1, "Test", atk1.m_AllEffects, 1);

    AttaqueType atk2 = SimpleHot();
    pm->AddGameEffectOnAtk("Test", atk2, "Test", atk2.m_AllEffects, 1);
    pm->ResetAllEffectsOnPlayer(&testCh);

    QCOMPARE(0, pm->m_AllEffectsOnGame["Test"].size());
}

int main(int argc, char *argv[]) {
  auto app = std::make_unique<Application>(argc, argv);
  app->Init();

  player_manager_tests test2;
  QTest::qExec(&test2);

  return 0;
}

#include "tst_players_manager.moc"
