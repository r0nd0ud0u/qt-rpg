#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QColor>
#include <QWidget>

#include "gamemanager.h"
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
  void ResetUi();
  void UpdateAtLoadGame(const GameManager *gm);
public slots:
  // slots which can be called by other windows
  void UpdateViews(const QString &name);
  void UpdateActivePlayers(const bool isLoadingGame, const GameManager *gm);
  void UpdateGameStatus(const GameState *gs);
signals:
  void selectCharacter(Character *);
  void SigUpdatePlayerPanel(
      const std::unordered_map<QString, std::vector<GameAtkEffects>> &);
  void SigBossDead(QString);
  void SigEndOfGame();
  void SigUpdateChannelView(const QString &, const QString &,
                            const QColor = QColor("purple"));
  void SigUpdStatsOnSelCharacter(Character *);
  void SigAddCharacter(Character *);
  void SigSetFocusOnActivePlayer(const Character *);
  void SigGameDisplayStart(Character *);

private:
  Ui::GameDisplay *ui;
  bool
  ProcessAtk(const TargetInfo *target, const AttaqueType &currentAtk,
             Character *activatedPlayer, const bool isCrit,
             const QString &nameChara,
             std::unordered_map<QString, std::vector<effectParam>> &newEffects);
  void UpdateViewAtRoundStart(Character *activePlayer,
                              const GameManager* gm);
private slots:
  void on_attaque_button_clicked();
  void on_bag_button_clicked();
  void on_stackedWidget_currentChanged(const int arg1);
  void StartNewTurn();
  void EndOfTurn();
  void EndOfGame();
  bool NewRound();
  void LaunchAttak(const QString &atkName, const std::vector<TargetInfo *> &);
  void on_mana_potion_button_clicked();
  void on_hp_potion_button_clicked();
  void on_berseck_potion_button_clicked();
  void on_vigor_potion_button_clicked();
  void on_add_exp_button_clicked();
  void SlotUpdateActionViews(const QString &name, const QString &form);
};

#endif // GAMEDISPLAY_H
