#ifndef HOSTPAGE_H
#define HOSTPAGE_H

#include <QWidget>

namespace Ui {
class HostPage;
}

class HostPage : public QWidget {
  Q_OBJECT

public:
  explicit HostPage(QWidget *parent = nullptr);
  ~HostPage();

  void ActiveNewGame(const bool value);
signals:
  void SigShowLoadGamePage();
  void SigShowHeroGameCharacters();
  void SigShowBossGameCharacters();

private slots:
  void on_newGamePushButton_clicked();

  void on_loadGamePushButton_clicked();

private:
  Ui::HostPage *ui;
};

#endif // HOSTPAGE_H
