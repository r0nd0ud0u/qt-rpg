#include "loadgame.h"
#include "ui_loadgame.h"

#include <QStringListModel>

LoadGame::LoadGame(QWidget *parent) : QWidget(parent), ui(new Ui::LoadGame) {
  ui->setupUi(this);
}

LoadGame::~LoadGame() { delete ui; }

void LoadGame::InitView(const QStringList &gamesList) {
  ui->btn_start_game->setEnabled(false);
  // Create model
  auto *model = new QStringListModel(this);
  // Populate our model
  model->setStringList(gamesList);

  // Glue model and view together
  ui->listView->setModel(model);
}

void LoadGame::on_btn_start_game_clicked() {
    emit SigStartGame(m_SelectedGame);
}

void LoadGame::on_listView_clicked(const QModelIndex &index) {
  ui->btn_start_game->setEnabled(true);
  m_SelectedGame = index.data().toString();
}
