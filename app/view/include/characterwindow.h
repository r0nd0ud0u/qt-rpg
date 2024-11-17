#ifndef CHARACTERWINDOW_H
#define CHARACTERWINDOW_H

#include <QMainWindow>

#include "character.h"

namespace Ui {
class CharacterWindow;
}

enum class actionType { defaultType = 0, newCharacter, edit, enumSize };

enum class tabType { character, attak, stuff, useStuff, enumSize };

class CharacterWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit CharacterWindow(QWidget *parent = nullptr);
  ~CharacterWindow();
  void InitWindow(const tabType &type, Character *c);
  void UpdateViewUseStuff();
  void UpdateViewAtGameStart();
  void UpdateViewAtNewGame();

signals:
  void SigUpdateCharacterViews(Character *);
  void SigUseNewStuff(const QString &);
  void SigUpdateViews(Character *);

private slots:
  void on_pushButton_clicked();

  void on_apply_pushButton_clicked();

private:
  Ui::CharacterWindow *ui;
  Character *m_CurCharacter = nullptr;

  void Apply();
};

#endif // CHARACTERWINDOW_H
