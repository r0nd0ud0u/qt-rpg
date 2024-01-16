#ifndef HEROESVIEW_H
#define HEROESVIEW_H

#include <QWidget>
#include <vector>

#include "character.h"

class HeroPanel;

namespace Ui {
class HeroesView;
}

class HeroesView : public QWidget
{
    Q_OBJECT

public:
    explicit HeroesView(QWidget *parent = nullptr);
    ~HeroesView();

signals:
    void SigAddStuff();
private slots:
    void Dosomething();

private:
    Ui::HeroesView *ui;

    void InitHeroPanel();
    std::vector<HeroPanel*> m_HeroPanels;
};

#endif // HEROESVIEW_H
