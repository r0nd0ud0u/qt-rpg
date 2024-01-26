#ifndef CHANNEL_H
#define CHANNEL_H

#include <QWidget>

namespace Ui {
class Channel;
}

class Channel : public QWidget {
  Q_OBJECT

public:
  explicit Channel(QWidget *parent = nullptr);
  ~Channel();

  void ShowPageStuffs();
  void ShowTurnPage();
  void AddStuff();
signals:
  void SigNextRound();
    void SigNewTurn();
  void SigEndOfTurn();
private slots:
  void on_next_turn_button_clicked();
  void on_end_round_button_clicked();
  void EndOfGame();

  private:
  Ui::Channel *ui;
};

#endif // CHANNEL_H
