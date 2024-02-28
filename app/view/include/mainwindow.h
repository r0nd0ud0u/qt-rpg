#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "character.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
public slots:
  void AddNewCharacter(Character *);
  void AddNewStuff();
  void UpdateStuffOnUse(const QString& playerName);

signals:
  void SigNewCharacter(Character *);
  void SigAddNewStuff();
  void SigNewStuffOnUse(const QString&);

private:
  Ui::MainWindow *ui;

private slots:
  void ShowPageGameDisplay();
};
#endif // MAINWINDOW_H
