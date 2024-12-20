#ifndef CHANNEL_H
#define CHANNEL_H

#include <QWidget>
#include <QColor>

namespace Ui {
class Channel;
}

class Channel : public QWidget {
  Q_OBJECT

public:
  explicit Channel(QWidget *parent = nullptr);
  ~Channel();
  void ResetUi();
  void UpdateTurnsButtons(const bool isNextTurn);

signals:
  void SigNextRound();
    void SigNewTurn();
  void SigEndOfTurn();
private slots:
  void on_next_turn_button_clicked();
  void on_end_round_button_clicked();
  void EndOfGame();
  void UpdateLog(const QString&, const QString& log, const QColor = QColor("dark"));

  private:
  Ui::Channel *ui;
};

#endif // CHANNEL_H
