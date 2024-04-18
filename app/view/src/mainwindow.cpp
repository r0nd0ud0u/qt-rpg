#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "Application.h"
#include "Stylizer.h"
#include "hostpage.h"

#include "gamedisplay.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  Stylizer::ApplyTheme(this);

  // Connect: Buttons on host page
  connect(ui->page, &HostPage::showGameDisplay, this,
          &MainWindow::ShowPageGameDisplay);
  connect(ui->page, &HostPage::SigShowGameCharacters, this,
          &MainWindow::ShowGameCharacters);
  connect(ui->page_3, &GameCharacters::SigBackToHostPage, this,
          &MainWindow::UpdateActiveCharacters);

  // Connect: Functions on secondary pages
  connect(this, &MainWindow::SigNewCharacter, ui->page_2,
          &GameDisplay::AddNewCharacter);
  connect(this, &MainWindow::SigNewCharacter, ui->page_2,
          &GameDisplay::AddNewStuff);
  connect(this, &MainWindow::SigNewStuffOnUse, ui->page_2,
          &GameDisplay::UpdateViews);
  connect(this, &MainWindow::SigUpdateActivePlayers, ui->page_2,
          &GameDisplay::UpdateActivePlayers);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::ShowPageGameDisplay() {
  if (const auto *gm = Application::GetInstance().m_GameManager.get(); gm != nullptr && gm->m_PlayersManager != nullptr) {
    gm->m_PlayersManager->UpdateActivePlayers(m_ActivePlayers);
  }
  emit SigUpdateActivePlayers();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::gameDisplay));
      static_cast<int>(SecondaryPages::gameDisplay));
}

void MainWindow::ShowGameCharacters() {
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::gameCharacters));
}

void MainWindow::ShowHostPage() {
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::hostPage));
}

void MainWindow::UpdateActiveCharacters(const std::set<QString> &playersList) {
  m_ActivePlayers = playersList;
  ShowHostPage();
}

void MainWindow::AddNewCharacter(Character *ch) { emit SigNewCharacter(ch); }

void MainWindow::AddNewStuff() { emit SigAddNewStuff(); }

void MainWindow::UpdateStuffOnUse(const QString &playerName) {
  emit SigNewStuffOnUse(playerName);
}
