#ifndef CHARACTERWINDOW_H
#define CHARACTERWINDOW_H

#include <QMainWindow>

#include "character.h"

namespace Ui {
class CharacterWindow;
}

enum class actionType { defaultType = 0, newCharacter, edit, enumSize };

enum class tabType {character, attak, stuff, useStuff, enumSize};

class CharacterWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit CharacterWindow(QWidget *parent = nullptr);
  ~CharacterWindow();
  void InitWindow(const tabType &type, const bool index = false);

  signals:
  void SigNewCharacter(Character*);
  void SigAddNewStuff();
  void SigUseNewStuff(const QString&);

private slots:
  void on_pushButton_clicked();

    void on_apply_pushButton_clicked();

  void on_tabWidget_currentChanged(int index);

  private:
  Ui::CharacterWindow *ui;
  Character* m_CurCharacter = nullptr;
};

#endif // CHARACTERWINDOW_H
