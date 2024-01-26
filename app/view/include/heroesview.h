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
    ~HeroesView() final;
    void ActivatePanel(const QString&);

signals:
    void SigAddStuff();
    void SigClickedOnHeroPanel(QString);
private slots:
    void Dosomething();
    void SlotClickedOnHeroPanel(QString name);

private:
    Ui::HeroesView *ui;

    void InitHeroPanel();
    std::vector<HeroPanel*> m_HeroPanels;
};

#endif // HEROESVIEW_H
