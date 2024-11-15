#include "hostpage.h"
#include "ui_hostpage.h"

HostPage::HostPage(QWidget *parent) : QWidget(parent), ui(new Ui::HostPage) {
  ui->setupUi(this);
}

HostPage::~HostPage() { delete ui; }

/**
 * @brief HostPage::ActiveNewGame
 * Enable new game button to start the game display page
 */
void HostPage::ActiveNewGame(const bool value) {
  ui->loadGamePushButton->setEnabled(value);
}

void HostPage::on_newGamePushButton_clicked() { emit SigShowHeroGameCharacters(); }

void HostPage::on_loadGamePushButton_clicked() { emit SigShowLoadGamePage(); }

