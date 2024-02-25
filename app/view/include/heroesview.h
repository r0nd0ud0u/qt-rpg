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
private slots:
  void Dosomething();
  void SlotClickedOnHeroPanel(const QString &name);
  void UpdateAllPanels();
  void SetFocusOn(const QString &name, const characType &);
  void UpdateSelected(const QString &name);

private:
  Ui::HeroesView *ui;

  void InitHeroPanel();
  std::vector<HeroPanel *> m_HeroPanels;
};

#endif // HEROESVIEW_H
