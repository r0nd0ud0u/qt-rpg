#ifndef BOSSESVIEW_H
#define BOSSESVIEW_H

#include "character.h"

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
    ~BossesView() final;
    void ActivatePanel(const QString&);
private:
    Ui::BossesView *ui;
    std::vector<BossPanel*> m_BossPanels;

    void InitBossPanels();

private slots:
    void UpdateAllPanels();
    void RemoveBoss(QString);
    void AddBossPanel(Character* ch);
    void SetFocusOn(const QString& name, const characType&);

};

#endif // BOSSESVIEW_H
