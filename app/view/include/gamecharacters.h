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

  void UpdateSelected(const Character*) const;
  void InitAllHeroesPanel();
  void InitAllBossesPanel();
  void SetTextNextButton(const QString& str);
  void ResetPages() const;
  void UpdatePanelAfterEdit(Character* c);
  void UpdateViews(Character* c);

signals:
  void SigDead(); // if i dont add this line; the call to 'emit
                  // SigBackBtnPushed' is crashing the app
  void SigBackBtnPushed(const bool);
  void SigSelectGameCharacter(Character*);
  void SigNextButtonPushed(const bool);
  void SigStartEditCharacter(const Character*);

private slots:
  void on_back_pushButton_clicked();
  void SelectPanel(Character*);
  void UpdateCharacterPlaying(const QString &name) const;

  void on_nextPushButton_clicked();

  void on_addCharaPushButton_clicked();

  private:
  Ui::GameCharacters *ui;
  std::vector<HeroPanel *> m_HeroesList;
  std::vector<BossPanel *> m_BossesList;
  bool m_IsHeroType = false;
  void ResetAllCharacterPlaying() const;
  void SetType(const bool value) { m_IsHeroType = value; }
  void AddCharacter();
  void InitHero(Character* c);
  void InitBoss(Character* c);
  void SetFocusOnBossPanel(const Character* c);
  void SetFocusOnHeroPanel(const Character* c);
  void SetFocusLastOnPanel();
  void RemoveHeroPanel(const HeroPanel* pnl);
  void RemoveBossPanel(const BossPanel* pnl);
};

#endif // GAMECHARACTERS_H
