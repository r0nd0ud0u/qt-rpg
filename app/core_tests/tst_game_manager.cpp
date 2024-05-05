#include <QDebug>
#include <QtTest>

#include "Application.h"
#include "playersmanager.h"

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

  game_manager_tests test4;
  QTest::qExec(&test4);


  return 0;
}

#include "tst_game_manager.moc"
