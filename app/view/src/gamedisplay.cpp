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
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
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
  gs->m_CurrentRound++;
  UpdateGameStatus();
  ui->heroes_widget->ActivatePanel(
      gs->m_OrderToPlay.at(gs->m_CurrentRound - 1));
  ui->bosses_widget->ActivatePanel(
      gs->m_OrderToPlay.at(gs->m_CurrentRound - 1));
  // Activate actions buttons
  ui->bag_button->setEnabled(true);
  ui->attaque_button->setEnabled(true);
  // default page on action view
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
  // TODO update channel
  // choice of talent
  // if dead -> choice to take a potion
}

void GameDisplay::StartNewTurn() {
  // TODO game state , check if boss is dead
  const auto &gm = Application::GetInstance().m_GameManager;
  // First process the order
  gm->ProcessOrderToPlay(gm->m_GameState->m_OrderToPlay);
  // Update game state
  gm->m_GameState->m_CurrentRound = 0;
  gm->m_GameState->m_CurrentTurnNb++;
  NewRound();
  // Then, update the display
  UpdateGameStatus();
}

void GameDisplay::EndOfNewTurn() {
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
  auto *hero = gm->m_PlayersManager->GetCharacterByName(nameChara);
  // launch atk
  for (const auto &target : targetList) {
    if (!target.m_IsTargeted) {
      continue;
    }
    auto *targetChara = gm->m_PlayersManager->GetCharacterByName(target.m_Name);

    if (hero != nullptr && targetChara != nullptr) {
      hero->Attaque(atkName, targetChara);
    }
  }
  // Stats change on hero
  if (hero != nullptr) {
    hero->StatsChangeAfterAtk(atkName);
  }
  // update views
  emit SigUpdateHeroPanel();
}
