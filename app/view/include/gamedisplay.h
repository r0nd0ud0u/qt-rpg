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
  void UpdateActivePlayers();
signals:
  void selectCharacter(const Character*);
  void SigUpdatePlayerPanel(const std::unordered_map<QString, std::vector<GameAtkEffects>> &);
  void SigBossDead(QString);
  void SigEndOfGame();
  void SigUpdateChannelView(const QString &, const QString &,
                            const QColor = QColor("purple"));
  void SigUpdateAllEffectPanel(
      const std::unordered_map<QString, std::vector<GameAtkEffects>> &);
  void SigUpdStatsOnSelCharacter(const Character*);
  void SigAddCharacter(Character *);
  void SigSetFocusOnActivePlayer(const Character*);
  void SigGameDisplayStart(const Character*);

private:
  Ui::GameDisplay *ui;

  void UpdateGameStatus();
  bool
  ProcessAtk(const TargetInfo *target, const AttaqueType &currentAtk,
             Character *activatedPlayer, const bool isCrit,
             const QString &nameChara,
             std::unordered_map<QString, std::vector<effectParam>> &newEffects);

private slots:
  void on_attaque_button_clicked();
  void on_bag_button_clicked();
  void on_stackedWidget_currentChanged(const int arg1);
  void StartNewTurn();
  void EndOfTurn();
  void EndOfGame();
  void NewRound();
  void LaunchAttak(const QString &atkName, const std::vector<TargetInfo *> &);
  void on_add_boss_button_clicked();
  void on_mana_potion_button_clicked();
  void on_hp_potion_button_clicked();
  void on_berseck_potion_button_clicked();
  void on_vigor_potion_button_clicked();
  void on_add_exp_button_clicked();
  void SlotUpdateActionViews(const QString &name, const QString &form);
};

#endif // GAMEDISPLAY_H
