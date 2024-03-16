#include <QtTest>
#include "character.h"

class character_tests : public QObject
{
    Q_OBJECT

public:

private slots:
    void character_init();

};

void character_tests::character_init()
{
    int a = 3;
    Q_ASSERT(a == 3);
}

#include "tst_character.moc"
