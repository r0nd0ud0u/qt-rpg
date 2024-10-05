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
  connect(ui->page, &HostPage::SigShowHeroGameCharacters, this,
          &MainWindow::RawDisplayHeroGameCh);
  connect(ui->page_Hero, &GameCharacters::SigBackBtnPushed, this,
          &MainWindow::ProcessGameCharacterBackBtn);
  connect(ui->page_Hero, &GameCharacters::SigNextButtonPushed, this,
          &MainWindow::ProcessGameCharacterNextBtn);
  connect(ui->page_Boss, &GameCharacters::SigNextButtonPushed, this,
          &MainWindow::ProcessGameCharacterNextBtn);
  connect(ui->page_Boss, &GameCharacters::SigBackBtnPushed, this,
          &MainWindow::ProcessGameCharacterBackBtn);

  // Connect: Functions on secondary pages
  connect(this, &MainWindow::SigNewStuffOnUse, ui->page_2,
          &GameDisplay::UpdateViews);
  connect(this, &MainWindow::SigUpdateActivePlayers, ui->page_2,
          &GameDisplay::UpdateActivePlayers);

  // init game characters
  ui->page_Hero->InitAllHeroesPanel();
  ui->page_Boss->InitAllBossesPanel();
}

MainWindow::~MainWindow() { delete ui; }

/**
 * @brief MainWindow::ShowPageGameDisplay
 * The game display will be shown only if at least one boss
 * and at least one hero have been chosen in "game characters" page beforehand
 */
void MainWindow::ShowPageGameDisplay() {
  if (const auto *gm = Application::GetInstance().m_GameManager.get();
      gm != nullptr && gm->m_PlayersManager != nullptr &&
      !gm->m_PlayersManager->UpdateActivePlayers()) {
    return;
  }
  emit SigUpdateActivePlayers();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::gameDisplay));
}

void MainWindow::RawDisplayHeroGameCh() { ShowHeroGameCharacters(true); }

void MainWindow::ShowHeroGameCharacters(const bool init) {
  if (init) {
    ui->page_Boss->ResetPages();
    ui->page_Hero->ResetPages();
  }
  ui->page_Hero->SetTextNextButton("Suivant");
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::heroGameCharacters));
}

void MainWindow::ShowBossGameCharacters() {
  ui->page_Boss->SetTextNextButton("Valider");
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::bossGameCharacters));
}

void MainWindow::ProcessGameCharacterNextBtn(const bool value) {
  if (value) {
    ShowBossGameCharacters();
  } else {
    UpdateActiveCharacters();
  }
}

void MainWindow::ProcessGameCharacterBackBtn(const bool value) {
  if (value) {
    ShowHostPage();
  } else {
    ShowHeroGameCharacters(false);
  }
}

void MainWindow::ShowHostPage() {
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::hostPage));
  ui->page->ActiveNewGame(true);
}

void MainWindow::UpdateActiveCharacters() { ShowPageGameDisplay(); }

void MainWindow::UpdateNewCharacter(Character *ch) {
    if(ch->m_type == characType::Hero){
        ui->page_Hero->UpdatePanelAfterEdit(ch);
    } else {
        ui->page_Boss->UpdatePanelAfterEdit(ch);
    }
}

void MainWindow::AddNewStuff() { emit SigAddNewStuff(); }

void MainWindow::UpdateStuffOnUse(const QString &playerName) {
  emit SigNewStuffOnUse(playerName);
}
