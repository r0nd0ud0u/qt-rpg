#include <QtTest>
#include "utils.h"

// add necessary includes here

class core_tests : public QObject
{
    Q_OBJECT

public:
    core_tests();
    ~core_tests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void GetRandomNb_works();
    void BuildEffectName_works();
    void CompareByLevel_works();

};

core_tests::core_tests()
{

}

core_tests::~core_tests()
{

}

void core_tests::initTestCase()
{

}

void core_tests::cleanupTestCase()
{

}

void core_tests::GetRandomNb_works()
{
    // test if output is between the min and max
    const auto a = Utils::GetRandomNb(0,100);
    Q_ASSERT(a >=0);
    Q_ASSERT(a <=100);
}

void core_tests::BuildEffectName_works(){
    // case args not empty
    QString str = Utils::BuildEffectName("effect", "stats");
    Q_ASSERT("stats-effect" == str);
    // case effect str empty
    str = Utils::BuildEffectName("", "stats");
    // case stats empty
    str = Utils::BuildEffectName("effect", "");
    Q_ASSERT("effect" == str);
    // case both args empty
    str = Utils::BuildEffectName("", "");
    Q_ASSERT("" == str);
}

void core_tests::CompareByLevel_works(){
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

QTEST_APPLESS_MAIN(core_tests)

#include "tst_core_tests.moc"
