#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QColor>
#include <QWidget>

#include "playersmanager.h"

// The different types of the actions to perform by a player
enum class ActionsStackedWgType { attak, defaultType, inventory, enumSize };

namespace Ui {
class GameDisplay;
}

class GameDisplay : public QWidget {
  Q_OBJECT

public:
  explicit GameDisplay(QWidget *parent = nullptr);
  ~GameDisplay();
  void AddNewCharacter(Character *);
  void AddNewStuff() const;
  public slots:
  // slots which can be called by other windows
  void UpdateViews(const QString &name);
signals:
  void selectCharacter(const QString&);
  void SigUpdatePlayerPanel();
  void SigBossDead(QString);
  void SigEndOfGame();
  void SigUpdateChannelView(const QString &, const QString &,
                            const QColor = QColor("purple"));
  void SigUpdateAllEffectPanel(
      const std::unordered_map<QString, std::vector<GameAtkEffects>> &);
  void SigUpdStatsOnSelCharacter();
  void SigAddCharacter(Character *);
  void SigSetFocusOnActivePlayer(const QString&, const characType&);

private:
  Ui::GameDisplay *ui;
  void UpdateGameStatus();

private slots:
  void UpdateChannel();
  void on_attaque_button_clicked();
  void on_bag_button_clicked();
  void on_stackedWidget_currentChanged(const int arg1);
  void StartNewTurn();
  void EndOfTurn();
  void EndOfGame();
  void NewRound();
  void LaunchAttak(const QString &atkName, const std::vector<TargetInfo*> &);
  void on_add_boss_button_clicked();
  void on_mana_potion_button_clicked();
  void on_hp_potion_button_clicked();
  void on_berseck_potion_button_clicked();
  void on_vigor_potion_button_clicked();
  void on_add_exp_button_clicked();
  void SlotUpdateActionViews(const QString& name,const QString& form);
};

#endif // GAMEDISPLAY_H
