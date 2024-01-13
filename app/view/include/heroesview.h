#ifndef HEROESVIEW_H
#define HEROESVIEW_H

#include <QWidget>
#include <vector>

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

private slots:
    void on_pushButton_clicked();

private:
    Ui::HeroesView *ui;

    std::vector<HeroPanel*> m_HeroesList;

    void AddHeroPanel();
};

#endif // HEROESVIEW_H
