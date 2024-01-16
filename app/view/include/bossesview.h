#ifndef BOSSESVIEW_H
#define BOSSESVIEW_H

#include <QWidget>

#include <vector>

class BossPanel;

namespace Ui {
class BossesView;
}

class BossesView : public QWidget
{
    Q_OBJECT

public:
    explicit BossesView(QWidget *parent = nullptr);
    ~BossesView();

private:
    Ui::BossesView *ui;
    std::vector<BossPanel*> m_BossPanels;

    void InitBossPanels();

};

#endif // BOSSESVIEW_H
