#ifndef GAMECHARACTERS_H
#define GAMECHARACTERS_H

#include <QWidget>

#include "bosspanel.h"
#include "heropanel.h"

namespace Ui {
class GameCharacters;
}

class GameCharacters : public QWidget {
  Q_OBJECT

public:
  explicit GameCharacters(QWidget *parent = nullptr);
  ~GameCharacters();

signals:
  void SigBackToHostPage(const std::set<QString> &);

private slots:
  void on_back_pushButton_clicked();
  void ActivatePanel(const QString &name) const;

private:
  Ui::GameCharacters *ui;
  void InitAllHeroesPanel();
  void InitAllBossesPanel();
  std::vector<HeroPanel *> m_HeroesList;
  std::vector<BossPanel *> m_BossesList;
};

#endif // GAMECHARACTERS_H
