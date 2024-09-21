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

  void UpdateSelected(const QString &name) const;

signals:
  void SigDead(); // if i dont add this line; the call to 'emit
                  // SigReturnToHostPage' is crashing the app
  void SigReturnToHostPage();
  void SigSelectGameCharacter(const QString &name, const QString &photoName);

private slots:
  void on_back_pushButton_clicked();
  void SelectPanel(const QString &name);
  void UpdateCharacterPlaying(const QString &name) const;

private:
  Ui::GameCharacters *ui;
  void InitAllHeroesPanel();
  void InitAllBossesPanel();
  std::vector<HeroPanel *> m_HeroesList;
  std::vector<BossPanel *> m_BossesList;
};

#endif // GAMECHARACTERS_H
