#include <QtTest>

#include "utils.h"
#include "playersmanager.h"

class utils_tests : public QObject
{
    Q_OBJECT


private slots:
    void CompareByLevel_works();

};


void utils_tests::CompareByLevel_works(){
    //init
    AttaqueType atk1;
    atk1.level =1;
    AttaqueType atk2;
    atk2.level =1;
    // case same level
    bool result = Utils::CompareByLevel(atk1, atk2);
    Q_ASSERT(!result);
    // case level1 < level2
    atk2.level =2;
    result = Utils::CompareByLevel(atk1, atk2);
    Q_ASSERT(result);
    // case level1 > level2
    atk1.level =3;
    result = Utils::CompareByLevel(atk1, atk2);
    Q_ASSERT(!result);
}

class player_manager_tests : public QObject
{
    Q_OBJECT

public:

private slots:
    void GetDeadliestAlly_works();
    void ProcessDamageTXHealNeedyAlly_works();
};

void player_manager_tests::GetDeadliestAlly_works(){

    PlayersManager pl;
    Character* c1 = new Character;
    Character* c2 = new Character;

    // case pv c1 < pv c2
    std::get<StatsType<int>>(
        c1->m_Stats.m_AllStatsTable.at(STATS_HP)).m_CurrentValue = 0;
    std::get<StatsType<int>>(
        c1->m_Stats.m_AllStatsTable.at(STATS_HP)).m_MaxValue = 1000;
    std::get<StatsType<int>>(
        c2->m_Stats.m_AllStatsTable.at(STATS_HP)).m_CurrentValue = 1000;
    std::get<StatsType<int>>(
        c2->m_Stats.m_AllStatsTable.at(STATS_HP)).m_MaxValue = 1000;
    c1->m_Name = "c1";
    c2->m_Name = "c2";
    pl.m_HeroesList.push_back(c1);
    pl.m_HeroesList.push_back(c2);

    const auto output = pl.GetAllDeadliestAllies(characType::Hero);
    QCOMPARE("c1", output.value().front());

    // case pv c1 == pv c2
    std::get<StatsType<int>>(
        c2->m_Stats.m_AllStatsTable.at(STATS_HP)).m_CurrentValue = 0;
    const auto output2 = pl.GetAllDeadliestAllies(characType::Hero).value();
    QCOMPARE("c1", output2.front());
    QCOMPARE("c2", output2.at(1));
}

void player_manager_tests::ProcessDamageTXHealNeedyAlly_works(){
    PlayersManager pl;
    Character* c1 = new Character;
    Character* c2 = new Character;

    // case pv c1 < pv c2
    std::get<StatsType<int>>(
        c1->m_Stats.m_AllStatsTable.at(STATS_HP)).m_CurrentValue = 0;
    std::get<StatsType<int>>(
        c1->m_Stats.m_AllStatsTable.at(STATS_HP)).m_MaxValue = 1000;
    std::get<StatsType<int>>(
        c2->m_Stats.m_AllStatsTable.at(STATS_HP)).m_CurrentValue = 1000;
    std::get<StatsType<int>>(
        c2->m_Stats.m_AllStatsTable.at(STATS_HP)).m_MaxValue = 1000;
    c1->m_Name = "c1";
    c2->m_Name = "c2";
    pl.m_HeroesList.push_back(c1);
    pl.m_HeroesList.push_back(c2);

    pl.ProcessDamageTXHealNeedyAlly(characType::Hero, 100);
    QCOMPARE(25,    std::get<StatsType<int>>(
                 c1->m_Stats.m_AllStatsTable.at(STATS_HP)).m_CurrentValue);
}

int main(int argc, char *argv[])
{
    utils_tests test;
    QTest::qExec(&test);

    player_manager_tests test2;
    QTest::qExec(&test2);

    return 0;
}


#include "tst_utils.moc"
