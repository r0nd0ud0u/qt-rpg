#include <QtTest>
#include "utils.h"
#include "character.h"

// add necessary includes here

class utils_tests : public QObject
{
    Q_OBJECT


private slots:
    void GetRandomNb_works();
    void CompareByLevel_works();

};

void utils_tests::GetRandomNb_works()
{
    // test if output is between the min and max
    const auto a = Utils::GetRandomNb(0,100);
    Q_ASSERT(a >=0);
    Q_ASSERT(a <=100);
}

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

class character_tests : public QObject
{
    Q_OBJECT

public:

private slots:
    void test_works();
};

void character_tests::test_works(){

}

int main(int argc, char *argv[])
{
    utils_tests test;
    QTest::qExec(&test);

    character_tests test2;
    QTest::qExec(&test2);

    return 0;
}


#include "tst_utils.moc"
