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
  AttaqueType atk1 =
      testCh.m_AttakList["mana-vigor-berseck-changepercent-hero"];
  auto pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  pm->AddGameEffectOnAtk("Test", atk1, "Test", atk1.m_AllEffects, 1);
  // empty table
  auto result = Utils::GetNbOfActiveEffects({});
  QVERIFY(!result.has_value());
  // filled table with one-turn effects
  result = Utils::GetNbOfActiveEffects(pm->m_AllEffectsOnGame["Test"]);
  QVERIFY(result.has_value());
  QCOMPARE(result.value().buf, 0);
  QCOMPARE(result.value().debuf, 0);
  QCOMPARE(result.value().hot, 0);
  QCOMPARE(result.value().dot, 0);
  QCOMPARE(result.value().oneTurnHotDot, 3);

  // Test with several effects
  pm->m_AllEffectsOnGame["Test"].clear();
  AttaqueType atk2 = testCh.m_AttakList["GetNbOfActiveEffects"];
  pm->AddGameEffectOnAtk("Test", atk2, "Test", atk2.m_AllEffects, 1);
  result = Utils::GetNbOfActiveEffects(pm->m_AllEffectsOnGame["Test"]);
  QVERIFY(result.has_value());
  QCOMPARE(result.value().buf, 3);
  QCOMPARE(result.value().debuf, 7);
  QCOMPARE(result.value().hot, 0);
  QCOMPARE(result.value().dot, 0);
  QCOMPARE(result.value().oneTurnHotDot, 2);
  const auto sumBufNbs = result.value().buf + result.value().debuf +
                         result.value().hot + result.value().dot +
                         result.value().oneTurnHotDot;
  QCOMPARE(atk2.m_AllEffects.size(), sumBufNbs);
}

int main(int argc, char *argv[]) {
  auto app = std::make_unique<Application>(argc, argv);
  app->Init();

  utils_tests test;
  QTest::qExec(&test);

  return 0;
}

#include "tst_utils.moc"
