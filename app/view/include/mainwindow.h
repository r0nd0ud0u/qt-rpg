#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "character.h"
#include "gamemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class SecondaryPages {
  hostPage = 0,
  gameDisplay,
  heroGameCharacters,
  bossGameCharacters,
  loadGame,
  enumSize
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
public slots:
  void UpdatCharacterViews(Character *);
  void UpdateStuffOnUse(const QString &playerName);

signals:
  void SigNewCharacter(Character *);
  void SigNewStuffOnUse(const QString &);

private:
  Ui::MainWindow *ui;

private slots:
  void PrepareAndShowPageGameDisplay(const GameManager *gm, const bool isLoadingGame);
  void ShowHeroGameCharacters(const bool init);
  void ShowBossGameCharacters();
  void ProcessGameCharacterNextBtn(const bool value);
  void ProcessGameCharacterBackBtn(const bool value);
  void ShowHostPage();
  void StartGameFromScratch();
  void RawDisplayHeroGameCh();
  void on_actionSave_triggered();
  void on_actionQuit_triggered();
  void ShowLoadGamePage();
  void LoadGame(const QString& gameName);
  void ResetGameDisplay();
};
#endif // MAINWINDOW_H
