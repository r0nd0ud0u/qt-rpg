#include <QtTest>
#include <qobject.h>

#include "Application.h"

#include "classes/attak.cpp"
#include "classes/character_fixtures.cpp"

class character_tests : public QObject {
  Q_OBJECT

private slots:
  void TestThalia_works();
  void GetMaxNbOfApplies_works();
  void ChangeByPercent_works();
  void GetBufDebufNumbers_works();
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

void character_tests::ChangeByPercent_works() {
  auto *testCh = GetTestCharacter();

  // attaque 1
  AttaqueType atk1 =
      testCh->m_AttakList["mana-vigor-berseck-changepercent-ennemy"];
  const int expectedMana =
      testCh->m_Stats.m_AllStatsTable[STATS_MANA].m_CurrentValue +
      atk1.m_AllEffects[0].value *
          testCh->m_Stats.m_AllStatsTable[STATS_MANA].m_MaxValue / 100;
  const int expectedVigor =
      testCh->m_Stats.m_AllStatsTable[STATS_VIGOR].m_CurrentValue +
      atk1.m_AllEffects[1].value *
          testCh->m_Stats.m_AllStatsTable[STATS_VIGOR].m_MaxValue / 100;
  const int expectedBerseck =
      testCh->m_Stats.m_AllStatsTable[STATS_BERSECK].m_CurrentValue +
      atk1.m_AllEffects[2].value *
          testCh->m_Stats.m_AllStatsTable[STATS_BERSECK].m_MaxValue / 100;
  const auto &[conditionsOk, resultEffects, appliedEffects] =
      testCh->ApplyAtkEffect(true, atk1, testCh, false);

  QCOMPARE(true, conditionsOk);
  QCOMPARE(expectedMana,
           testCh->m_Stats.m_AllStatsTable[STATS_MANA].m_CurrentValue);
  QCOMPARE(expectedVigor,
           testCh->m_Stats.m_AllStatsTable[STATS_VIGOR].m_CurrentValue);
  QCOMPARE(expectedBerseck,
           testCh->m_Stats.m_AllStatsTable[STATS_BERSECK].m_CurrentValue);
}

void character_tests::GetMaxNbOfApplies_works() {
  Stats stats;
  stats.m_AllStatsTable[STATS_MANA].InitValues(100, 100);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(100, 100);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(100, 100);
  auto *c = new Character("", characType::Hero, stats);
  auto result = c->GetMaxNbOfApplies(SimpleAtkMana());
  QCOMPARE(10, result);
  result = c->GetMaxNbOfApplies(SimpleAtkVigor());
  QCOMPARE(10, result);
  result = c->GetMaxNbOfApplies(SimpleAtkBerseck());
  QCOMPARE(10, result);
}

void character_tests::GetBufDebufNumbers_works() {
  auto *testCh = GetTestCharacter();
  auto result = testCh->GetBufDebufNumbers();
  QVERIFY(result.has_value());

  for (int i = 0; i < static_cast<int>(BufTypes::enumSize); i++) {
    if (i == static_cast<int>(BufTypes::damageRx)) {
      testCh->m_AllBufs[i]->set_buffers(-10, false);
    } else {
      testCh->m_AllBufs[i]->set_buffers(10, false);
    }
  }
  result = testCh->GetBufDebufNumbers();
  QVERIFY(result.has_value());
  QCOMPARE(result->hot, 0);
  QCOMPARE(result->dot, 0);
  QCOMPARE(result->debuf, 0);
  QCOMPARE(result->buf, static_cast<int>(BufTypes::enumSize));
}

int main(int argc, char *argv[]) {
  auto app = std::make_unique<Application>(argc, argv);
  app->Init();

  character_tests test3;
  QTest::qExec(&test3);

  return 0;
}

#include "tst_characters.moc"
