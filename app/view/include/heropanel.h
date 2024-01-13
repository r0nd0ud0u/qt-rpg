#ifndef HEROPANEL_H
#define HEROPANEL_H

#include <QWidget>

#include "character.h"

namespace Ui {
class HeroPanel;
}

class HeroPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HeroPanel(QWidget *parent = nullptr);
    ~HeroPanel();

    Character* m_Heroe = nullptr;

private:
    Ui::HeroPanel *ui;
};

#endif // HEROPANEL_H
