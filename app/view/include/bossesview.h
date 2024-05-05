#ifndef BOSSESVIEW_H
#define BOSSESVIEW_H

#include "character.h"

#include <QWidget>

#include <vector>

class BossPanel;

namespace Ui {
class BossesView;
}

class BossesView : public QWidget {
  Q_OBJECT

public:
  explicit BossesView(QWidget *parent = nullptr);
  ~BossesView() final;
  void ActivatePanel(const QString &);

signals:
  void SigClickedOnPanel(const QString &);

private:
  Ui::BossesView *ui;
  std::vector<BossPanel *> m_BossPanels;

private slots:
  void UpdateAllPanels(const std::unordered_map<QString, std::vector<GameAtkEffects>> &);
  void RemoveBoss(QString);
  void AddBossPanel(Character *ch);
  void SetFocusOn(const QString &name, const characType &);
  void SlotClickedOnPanel(const QString &);
  void UpdateSelected(const QString &name) const;
  void InitBossPanels();
  ;
};

#endif // BOSSESVIEW_H
