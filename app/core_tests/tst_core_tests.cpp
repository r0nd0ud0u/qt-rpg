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
    void test_case1();

};

core_tests::core_tests()
{

}

core_tests::~core_tests()
{

}

void core_tests::initTestCase()
{
    const auto a = Utils::GetRandomNb(0,100);
    Q_ASSERT(a >=0);
    Q_ASSERT(a <=100);

}

void core_tests::cleanupTestCase()
{

}

void core_tests::test_case1()
{

}

QTEST_APPLESS_MAIN(core_tests)

#include "tst_core_tests.moc"
