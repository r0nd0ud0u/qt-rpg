#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "actionsview.h"
#include "channel.h"
#include "heroesview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

  connect(ui->heroes_widget, &HeroesView::SigAddStuff, this,
          &GameDisplay::UpdateChannel);
  connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this,
          &GameDisplay::UpdateViews);
  connect(ui->channel_lay, &Channel::SigNextRound, this,
          &GameDisplay::NewRound);
  connect(ui->channel_lay, &Channel::SigNewTurn, this,
          &GameDisplay::StartNewTurn);
  connect(ui->channel_lay, &Channel::SigEndOfTurn, this,
          &GameDisplay::StartNewTurn);
  connect(ui->attak_page, &ActionsView::SigLaunchAttak, this,
          &GameDisplay::LaunchAttak);

  // init display default page
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
  // init status page
  ui->turn_label->setText("Tour 0");
}

GameDisplay::~GameDisplay() { delete ui; }

void GameDisplay::UpdateChannel() {
  ui->channel_lay->ShowPageStuffs();
  ui->channel_lay->AddStuff();
}

void GameDisplay::UpdateViews(const QString &name) {
  const auto &app = Application::GetInstance();
  // lambda function to add here
  for (auto *hero : app.m_GameManager->m_PlayersManager->m_HeroesList) {
    if (hero->m_Name == name) {
      app.m_GameManager->m_PlayersManager->m_SelectedHero = hero;
      break;
    }
  }
  emit selectCharacter(name);
}

void GameDisplay::on_attaque_button_clicked() {
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::attak));
  ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(true);
}

void GameDisplay::on_bag_button_clicked() {
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::inventory));
  ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(true);
}

void GameDisplay::on_stackedWidget_currentChanged(const int arg1) {
  ui->attak_page->UpdateActions(static_cast<ActionsStackedWgType>(arg1));
}

void GameDisplay::UpdateGameStatus() {
  const auto &gameState = Application::GetInstance().m_GameManager->m_GameState;
  ui->turn_label->setText(QString("Tour %1, Round %2/%3")
                              .arg(gameState->m_CurrentTurnNb)
                              .arg(gameState->m_CurrentRound)
                              .arg(gameState->m_OrderToPlay.size()));
}

void GameDisplay::NewRound() {
  const auto &gs = Application::GetInstance().m_GameManager->m_GameState;
  // TODO game state , check if boss is dead

  // First update the game state
  gs->m_CurrentRound++;
  UpdateGameStatus();

  // Get current player
  auto *activePlayer =
      Application::GetInstance().m_GameManager->GetCurrentPlayer();

  // Update views
  // Players panels views
  ui->heroes_widget->ActivatePanel(activePlayer->m_Name);
  ui->bosses_widget->ActivatePanel(activePlayer->m_Name);
  // Activate actions buttons
  ui->bag_button->setEnabled(true);
  ui->attaque_button->setEnabled(true);
  // default page on action view
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
  // actions views
  ui->attak_page->SetCurrentPlayer(activePlayer);
  ui->inventory_page->SetCurrentPlayer(activePlayer);

  // TODO update channel
  // choice of talent
  // if dead -> choice to take a potion
}

void GameDisplay::StartNewTurn() {
  // TODO game state , check if boss is dead
  const auto &gm = Application::GetInstance().m_GameManager;

  // For each turn now
  // Process the order of the players
  gm->ProcessOrderToPlay(gm->m_GameState->m_OrderToPlay);
  // Update game state
  gm->m_GameState->m_CurrentRound = 0;
  gm->m_GameState->m_CurrentTurnNb++;
  NewRound();
  // Then, update the display
  UpdateGameStatus();
  // game is just starting at turn 1
  // some first init to do for the viewa
  if (gm->m_GameState->m_CurrentTurnNb == 1) {
    ui->attak_page->InitTargetsWidget();
  }
}

void GameDisplay::EndOfGame() {
  // Desactivate actions buttons
  ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(false);
  // default page on action view
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
}

void GameDisplay::LaunchAttak(const QString &atkName,
                              const std::vector<TargetInfo> &targetList) {
  const auto &gm = Application::GetInstance().m_GameManager;

  // Desactivate actions buttons
  ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(false);
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));

  const auto &nameChara = gm->m_GameState->GetCurrentPlayerName();
  auto *activatedHero = gm->m_PlayersManager->GetCharacterByName(nameChara);
  // launch atk
  for (const auto &target : targetList) {
    if (!target.m_IsTargeted) {
      continue;
    }
    auto *targetChara = gm->m_PlayersManager->GetCharacterByName(target.m_Name);

    if (activatedHero != nullptr && targetChara != nullptr) {
      activatedHero->Attaque(atkName, targetChara);
    }
  }
  // Stats change on hero
  if (activatedHero != nullptr) {
    activatedHero->StatsChangeAfterAtk(atkName);
  }
  // update views
  emit SigUpdateHeroPanel();

  // check who is dead!
  for (auto &boss : gm->m_PlayersManager->m_BossesList) {
      if (boss->m_Stats.m_HP.m_CurrentValue == 0) {
      // next phase
      emit SigBossDead(boss->m_Name);
      // delete bosses in player manager
      delete boss;
      boss = nullptr;
    }
  }
  // Check end of game
  if (gm->m_PlayersManager->m_BossesList.empty()) {
    // update buttons
    EndOfGame();
  }

  uint8_t nbDeadHeroes = 0;
  for (const auto &hero : gm->m_PlayersManager->m_HeroesList) {
      if (hero->m_Stats.m_HP.m_CurrentValue == 0) {
      // choose to drink a potion
      nbDeadHeroes++;
    }
  }
  if (nbDeadHeroes == gm->m_PlayersManager->m_HeroesList.size()) {
    // end of game
  }
}
