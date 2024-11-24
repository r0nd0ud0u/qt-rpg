#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "Application.h"
#include "ApplicationView.h"
#include "Stylizer.h"
#include "hostpage.h"

#include "gamedisplay.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  Stylizer::ApplyTheme(this);

  // Connect: Buttons on host page
  connect(ui->page, &HostPage::SigShowLoadGamePage, this,
          &MainWindow::ShowLoadGamePage);
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
  connect(ui->page_LoadGame, &LoadGame::SigStartGame, this,
          &MainWindow::LoadGame);

  // Connect: Functions on secondary pages
  connect(this, &MainWindow::SigNewStuffOnUse, ui->page_2,
          &GameDisplay::UpdateViews);
  // init game characters
  ui->page_Hero->InitAllHeroesPanel();
  ui->page_Boss->InitAllBossesPanel();
  ui->actionSave->setEnabled(false);
  ui->actionQuit->setEnabled(false);
}

MainWindow::~MainWindow() { delete ui; }

/**
 * @brief MainWindow::PrepareAndShowPageGameDisplay
 * The game display will be shown only if at least one boss
 * and at least one hero have been chosen in "game characters" page beforehand
 */
void MainWindow::PrepareAndShowPageGameDisplay(const GameManager *gm, const bool isLoadingGame) {
  if (gm == nullptr) {
    return;
  }
  ui->page_2->UpdateGameStatus(gm->m_GameState);

  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::gameDisplay));
  ui->actionSave->setEnabled(true);
  ui->actionQuit->setEnabled(true);
  auto &appView = ApplicationView::GetInstance();
  appView.GetCharacterWindow()->UpdateViewAtGameStart();
  ui->page_2->UpdateActivePlayers(isLoadingGame, gm);
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
  ui->actionSave->setEnabled(false);
  ui->actionQuit->setEnabled(true);

  // start game
  Application::GetInstance().m_GameManager->StartGame();
  auto &appView = ApplicationView::GetInstance();
  appView.GetCharacterWindow()->UpdateViewAtNewGame();
}

void MainWindow::ShowBossGameCharacters() {
  ui->page_Boss->SetTextNextButton("Valider");
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::bossGameCharacters));
  ui->actionSave->setEnabled(false);
  ui->actionQuit->setEnabled(true);
}

void MainWindow::ProcessGameCharacterNextBtn(const bool value) {
  if (value) {
    ShowBossGameCharacters();
  } else {
      StartGameFromScratch();
    setWindowState(Qt::WindowMaximized);
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
  auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm != nullptr) {
    gm->Reset();
  }
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::hostPage));
  ui->page->ActiveNewGame(true);
  ui->actionSave->setEnabled(false);
  ui->actionQuit->setEnabled(false);
}

void MainWindow::StartGameFromScratch() {
  const auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm == nullptr || gm->m_PlayersManager == nullptr) {
    return;
  }
  if (!gm->m_PlayersManager->UpdateStartingPlayers(false)) {
    return;
  }
  // 1st hero is selected to start the game
  if (!gm->m_PlayersManager->m_HeroesList.empty()) {
    gm->m_PlayersManager->m_SelectedPlayer =
        gm->m_PlayersManager->m_HeroesList.front();
  }
  PrepareAndShowPageGameDisplay(gm, false);
}

void MainWindow::UpdatCharacterViews(Character *ch) {
    if (ch->m_Type == characType::Hero) {
    ui->page_Hero->UpdatePanelAfterEdit(ch);
    ui->page_Hero->UpdateViews(ch);
  } else {
    ui->page_Boss->UpdatePanelAfterEdit(ch);
    ui->page_Boss->UpdateViews(ch);
  }
}

void MainWindow::UpdateStuffOnUse(const QString &playerName) {
  emit SigNewStuffOnUse(playerName);
}

void MainWindow::on_actionSave_triggered() {
  // save jsons in dir
  auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm != nullptr) {
    gm->SaveGame();
  }
}

void MainWindow::on_actionQuit_triggered() {
  ResetGameDisplay();
  ShowHostPage();
}

void MainWindow::ShowLoadGamePage() {
  ui->actionSave->setEnabled(false);
  ui->actionQuit->setEnabled(true);
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(SecondaryPages::loadGame));
  const auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm != nullptr) {
    ui->page_LoadGame->InitView(gm->GetListOfGames());
  }
}

void MainWindow::LoadGame(const QString &gameName) {
  if (gameName.isEmpty()) {
    return;
  }
  auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm == nullptr) {
    return;
  }
  if (gm->LoadGame(gameName)) {
    PrepareAndShowPageGameDisplay(gm, true);
  }
}

void MainWindow::ResetGameDisplay() { ui->page_2->ResetUi(); }
