#ifndef HEROESVIEW_H
#define HEROESVIEW_H

#include <QWidget>
#include <vector>

#include "character.h"

class HeroPanel;

namespace Ui {
class HeroesView;
}

class HeroesView : public QWidget {
  Q_OBJECT

public:
  explicit HeroesView(QWidget *parent = nullptr);
  ~HeroesView() final;
  void ActivatePanel(const QString &);

signals:
  void SigAddStuff();
  void SigClickedOnHeroPanel(const QString &);
  void SigSelectedFormOnHeroPanel(const QString&E,const QString &);
private slots:
  void Dosomething();
  void SlotClickedOnHeroPanel(const QString &name);
  void SlotSelectedForm(const QString &name,const QString &form);
  void UpdateAllPanels();
  void SetFocusOn(const QString &name, const characType &);
  void UpdateSelected(const QString &name) const;

private:
  Ui::HeroesView *ui;

  void InitHeroPanel();
  std::vector<HeroPanel *> m_HeroPanels;
};

#endif // HEROESVIEW_H
