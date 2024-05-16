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
  void InitAggroOnTurn_works();
  void SetStatsOnEffect_works();
  void TestAtkBlocked();
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
  auto testCh = GetTestCharacter();
  // attaque 1
  AttaqueType atk1 =
      testCh.m_AttakList["mana-vigor-berseck-changepercent-ennemy"];
  const int expectedMana =
      testCh.m_Stats.m_AllStatsTable[STATS_MANA].m_CurrentValue +
      atk1.m_AllEffects[0].value *
          testCh.m_Stats.m_AllStatsTable[STATS_MANA].m_MaxValue / 100;
  const int expectedVigor =
      testCh.m_Stats.m_AllStatsTable[STATS_VIGOR].m_CurrentValue +
      atk1.m_AllEffects[1].value *
          testCh.m_Stats.m_AllStatsTable[STATS_VIGOR].m_MaxValue / 100;
  const int expectedBerseck =
      testCh.m_Stats.m_AllStatsTable[STATS_BERSECK].m_CurrentValue +
      atk1.m_AllEffects[2].value *
          testCh.m_Stats.m_AllStatsTable[STATS_BERSECK].m_MaxValue / 100;
  const auto &[conditionsOk, resultEffects, appliedEffects] =
      testCh.ApplyAtkEffect(true, atk1, &testCh, false);

  QCOMPARE(true, conditionsOk);
  QCOMPARE(expectedMana,
           testCh.m_Stats.m_AllStatsTable[STATS_MANA].m_CurrentValue);
  QCOMPARE(expectedVigor,
           testCh.m_Stats.m_AllStatsTable[STATS_VIGOR].m_CurrentValue);
  QCOMPARE(expectedBerseck,
           testCh.m_Stats.m_AllStatsTable[STATS_BERSECK].m_CurrentValue);
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
  auto testCh = GetTestCharacter();
  auto result = testCh.GetBufDebufNumbers();
  QVERIFY(result.has_value());

  for (int i = 0; i < static_cast<int>(BufTypes::enumSize); i++) {
    if (i == static_cast<int>(BufTypes::damageRx)) {
      testCh.m_AllBufs[i]->set_buffers(-10, false);
    } else {
      testCh.m_AllBufs[i]->set_buffers(10, false);
    }
  }
  result = testCh.GetBufDebufNumbers();
  QVERIFY(result.has_value());
  QCOMPARE(result->hot, 0);
  QCOMPARE(result->dot, 0);
  QCOMPARE(result->debuf, 0);
  QCOMPARE(result->buf, static_cast<int>(BufTypes::enumSize));

  // reset
  for (int i = 0; i < static_cast<int>(BufTypes::enumSize); i++) {
      testCh.m_AllBufs[i]->set_buffers(0, false);
  }
}

void character_tests::InitAggroOnTurn_works() {
  auto testCh = GetTestCharacter();

  testCh.InitAggroOnTurn(0);
  const auto &aggroStat =
      testCh.m_Stats.m_AllStatsTable[STATS_AGGRO].m_CurrentValue;
  QCOMPARE(aggroStat, 0);

  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][0] = 0;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][1] = 10;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][2] = 10;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][3] = 10;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][4] = 10;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][5] = 10;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][6] = 0;
  testCh.m_LastTxRx[static_cast<int>(amountType::aggro)][7] = 0;

  testCh.InitAggroOnTurn(6);
  QCOMPARE(aggroStat, 50);

  testCh.InitAggroOnTurn(7);
  QCOMPARE(aggroStat, 40);
}

void character_tests::SetStatsOnEffect_works(){
    Stats stats;
    auto& hp = stats.m_HP;
    //init current value
    hp.m_MaxValue = 100;
    hp.m_CurrentValue = 50; // ratio 0.5 between cur value and max value
    hp.m_BufEffectPercent = 0;
    hp.m_BufEffectValue = 0;

    // raw value 0 => stat is not changing
    hp.m_RawMaxValue = 0;
    Character::SetStatsOnEffect(hp, 10, true, false);
    QCOMPARE(100, hp.m_MaxValue);
    QCOMPARE(50, hp.m_CurrentValue);
    QCOMPARE(0, hp.m_RawMaxValue);

    // no update of effect
    hp.m_RawMaxValue = 100;
    Character::SetStatsOnEffect(hp, 10, true, false);
    QCOMPARE(100, hp.m_MaxValue);
    QCOMPARE(50, hp.m_CurrentValue);
    // effect
    // percent
    //init current value
    hp.m_MaxValue = 100;
    hp.m_CurrentValue = 50; // ratio 0.5 between cur value and max value
    hp.m_BufEffectPercent = 0;
    hp.m_BufEffectValue = 0;
    Character::SetStatsOnEffect(hp, 10, true, true);
    QCOMPARE(110, hp.m_MaxValue);
    QCOMPARE(55, hp.m_CurrentValue);
    // value
    //init current value
    hp.m_MaxValue = 100;
    hp.m_CurrentValue = 50; // ratio 0.5 between cur value and max value
    hp.m_BufEffectPercent = 0;
    hp.m_BufEffectValue = 0;
    Character::SetStatsOnEffect(hp, 10, false, true);
    QCOMPARE(110, hp.m_MaxValue);
    QCOMPARE(55, hp.m_CurrentValue);

    // negative value
    // percent
    //init current value
    hp.m_MaxValue = 100;
    hp.m_CurrentValue = 50; // ratio 0.5 between cur value and max value
    hp.m_BufEffectPercent = 0;
    hp.m_BufEffectValue = 0;
    Character::SetStatsOnEffect(hp, -10, true, true);
    QCOMPARE(90, hp.m_MaxValue);
    QCOMPARE(45, hp.m_CurrentValue);
    // value
    //init current value
    hp.m_MaxValue = 100;
    hp.m_CurrentValue = 50; // ratio 0.5 between cur value and max value
    hp.m_BufEffectPercent = 0;
    hp.m_BufEffectValue = 0;
    Character::SetStatsOnEffect(hp, -10, false, true);
    QCOMPARE(90, hp.m_MaxValue);
    QCOMPARE(45, hp.m_CurrentValue);
}

void character_tests::TestAtkBlocked() {
    // boss
    auto testCh = GetTestCharacter();
    testCh.m_type = characType::Boss;
    // hero
    auto ch2 = GetTestCharacter();
    ch2.m_type = characType::Hero;
    // to make calculs easy on mana at
    testCh.m_Stats.m_AllStatsTable[STATS_POW_MAG].m_CurrentValue = 0;
    ch2.m_Stats.m_AllStatsTable[STATS_ARM_MAG].m_CurrentValue = 0;
    // init hp
    ch2.m_Stats.m_AllStatsTable[STATS_HP].m_CurrentValue = 50;
    ch2.m_Stats.m_AllStatsTable[STATS_HP].m_MaxValue = 50;
    const auto &[conditionsOk, resultEffects, appliedEffects] =
        testCh.ApplyAtkEffect(true, SimpleAtkMana(), &ch2, false);

    QCOMPARE(40, ch2.m_Stats.m_AllStatsTable[STATS_HP].m_CurrentValue);

    ch2.m_IsBlockingAtk = true;
    const auto &[conditionsOk2, resultEffects2, appliedEffects2] =
        testCh.ApplyAtkEffect(true, SimpleAtkMana(), &ch2, false);

    // 10% of 10 = 1 => 40 -1 = 39
    QCOMPARE(39, ch2.m_Stats.m_AllStatsTable[STATS_HP].m_CurrentValue);
}

int main(int argc, char *argv[]) {
  auto app = std::make_unique<Application>(argc, argv);
  app->Init();

  character_tests test3;
  QTest::qExec(&test3);

  return 0;
}

#include "tst_characters.moc"
