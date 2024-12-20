#include <QDebug>
#include <QtTest>

#include "Application.h"
#include "utils.h"

// fixtures
#include "classes/character_fixtures.cpp"

class utils_tests : public QObject {
  Q_OBJECT

private slots:
  void CompareByLevel_works();
  void GetNbOfActiveEffects_works();
  void GetRandomNb_works();
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

void utils_tests::GetNbOfActiveEffects_works() {
  auto testCh = GetTestCharacter();
  AttaqueType &atk1 =
      testCh.m_AttakList["mana-vigor-berseck-changepercent-hero"];
  auto pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  pm->AddGameEffectOnAtk("Test", atk1, "Test", atk1.m_AllEffects, 1);
  // empty table
  auto result = Utils::GetActiveEffects({});
  QVERIFY(!result.has_value());
  // filled table with one-turn effects
  result = Utils::GetActiveEffects(pm->m_AllEffectsOnGame["Test"]);
  QVERIFY(result.has_value());
  QCOMPARE(result.value().buf.nb, 0);
  QCOMPARE(result.value().debuf.nb, 0);
  QCOMPARE(result.value().hot.nb, 0);
  QCOMPARE(result.value().dot.nb, 0);
  QCOMPARE(result.value().oneTurnEffect, 3);

  // Test with several effects
  pm->m_AllEffectsOnGame["Test"].clear();
  AttaqueType atk2 = testCh.m_AttakList["GetNbOfActiveEffects"];
  pm->AddGameEffectOnAtk("Test", atk2, "Test", atk2.m_AllEffects, 1);
  result = Utils::GetActiveEffects(pm->m_AllEffectsOnGame["Test"]);
  QVERIFY(result.has_value());
  // mana changement par valeur, value > 0, turns: 2
  // pv up par valeur, value > 0, turns: 2
  // mana up par %, value > 0, turns: 2
  QCOMPARE(result.value().buf.nb, 3);
  // mana changement par valeur, value < 0, turns: 2
  // mana up par %, value < 0, turns: 2
  // pv up par valeur, value < 0, turns: 2
  QCOMPARE(result.value().debuf.nb, 3);
  // PV changement par valeur, value > 0, turns: 2
  QCOMPARE(result.value().hot.nb, 1);
  // PV changement par valeur, value < 0, turns: 2
  QCOMPARE(result.value().dot.nb, 1);
  // PV changement par valeur, value < 0, turns: 1
  // PV changement par valeur, value > 0, turns: 1
  // mana changement par valeur, value < 0, turns: 1
  // mana changement par valeur, value > 0, turns: 1
  QCOMPARE(result.value().oneTurnEffect, 4);
  const auto sumBufNbs = result.value().buf.nb + result.value().debuf.nb +
                         result.value().hot.nb + result.value().dot.nb +
                         result.value().oneTurnEffect;
  QCOMPARE(atk2.m_AllEffects.size(), sumBufNbs);
}

void utils_tests::GetRandomNb_works() {
  auto result = Utils::GetRandomNb(0, 100);
  QVERIFY(result >= 0);
  QVERIFY(result <= 100);

  result = Utils::GetRandomNb(0, 0);
  QVERIFY(result == 0);
}

int main(int argc, char *argv[]) {
  auto app = std::make_unique<Application>(argc, argv);
  app->Init();

  utils_tests test;
  QTest::qExec(&test);

  return 0;
}

#include "tst_utils.moc"
