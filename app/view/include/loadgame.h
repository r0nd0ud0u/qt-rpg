#ifndef LOADGAME_H
#define LOADGAME_H

#include <QWidget>

namespace Ui {
class LoadGame;
}

class LoadGame : public QWidget {
  Q_OBJECT

public:
  explicit LoadGame(QWidget *parent = nullptr);
  ~LoadGame();
  void InitView(const QStringList &gamesList);

signals:
  void SigStartGame(const QString& gameName);
private slots:
  void on_btn_start_game_clicked();

  void on_listView_clicked(const QModelIndex &index);

private:
  Ui::LoadGame *ui;
  QString m_SelectedGame;
};

#endif // LOADGAME_H
