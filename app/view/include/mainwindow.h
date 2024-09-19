#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "character.h"

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
  enumSize
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
public slots:
  void AddNewCharacter(Character *);
  void AddNewStuff();
  void UpdateStuffOnUse(const QString &playerName);

signals:
  void SigNewCharacter(Character *);
  void SigAddNewStuff();
  void SigNewStuffOnUse(const QString &);
  void SigUpdateActivePlayers();

private:
  Ui::MainWindow *ui;
  std::set<QString> m_ActivePlayers;

private slots:
  void ShowPageGameDisplay();
  void ShowGameCharacters();
  void ShowHostPage();
  void UpdateActiveCharacters(const std::set<QString> &);
};
#endif // MAINWINDOW_H
