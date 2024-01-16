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
    void UpdatePanel(Character* hero);
    void SetActive(bool activated);

    Character* m_Heroe = nullptr;

private slots:
    void on_pushButton_clicked();

signals:
    void addStuff();

private:
    Ui::HeroPanel *ui;

};

#endif // HEROPANEL_H
