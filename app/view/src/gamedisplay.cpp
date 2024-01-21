#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "heroesview.h"
#include "actionsview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

  connect(ui->heroes_widget, &HeroesView::SigAddStuff, this,
          &GameDisplay::UpdateChannel);
  connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this,
          &GameDisplay::UpdateViews);

  // init display default page
  ui->stackedWidget->setCurrentIndex(static_cast<int>(ActionsStackedWgType::defaultType));
}

GameDisplay::~GameDisplay() { delete ui; }

void GameDisplay::UpdateChannel() {
  ui->channel_lay->ShowPageStuffs();
  ui->channel_lay->AddStuff();
}

void GameDisplay::UpdateViews(const QString &name) {
  ui->stackedWidget->setCurrentIndex(static_cast<int>(ActionsStackedWgType::defaultType));
    ui->attaque_button->setEnabled(true);
    ui->bag_button->setEnabled(true);
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
    ui->stackedWidget->setCurrentIndex(static_cast<int>(ActionsStackedWgType::attak));
    ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(true);
}

void GameDisplay::on_bag_button_clicked() {
  ui->stackedWidget->setCurrentIndex(static_cast<int>(ActionsStackedWgType::inventory));
    ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(true);
}

void GameDisplay::on_stackedWidget_currentChanged(const int arg1 ) {
  ui->attak_page->UpdateView(static_cast<ActionsStackedWgType>(arg1));
}
