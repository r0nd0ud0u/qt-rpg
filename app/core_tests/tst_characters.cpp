#include <QtTest>
#include <qobject.h>

#include "Application.h"
#include "classes/attak.cpp"

class character_tests : public QObject {
    Q_OBJECT

private slots:
    void TestThalia_works();
    void GetMaxNbOfApplies_works();
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

void character_tests::GetMaxNbOfApplies_works() {
    Stats stats;
    stats.m_AllStatsTable[STATS_MANA].InitValues(100, 100);
    stats.m_AllStatsTable[STATS_VIGOR].InitValues(100, 100);
    stats.m_AllStatsTable[STATS_BERSECK].InitValues(100, 100);
    auto* c = new Character("",characType::Hero, stats);
    auto result = c->GetMaxNbOfApplies(SimpleAtkMana());
    QCOMPARE(10, result);
    result = c->GetMaxNbOfApplies(SimpleAtkVigor());
    QCOMPARE(10, result);
    result = c->GetMaxNbOfApplies(SimpleAtkBerseck());
    QCOMPARE(10, result);
}


int main(int argc, char *argv[]) {
    auto app = std::make_unique<Application>(argc, argv);
    app->Init();

    character_tests test3;
    QTest::qExec(&test3);

    return 0;
}

#include "tst_characters.moc"
