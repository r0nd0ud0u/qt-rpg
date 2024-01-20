#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "heroesview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

  connect(ui->heroes_widget, &HeroesView::SigAddStuff, this,
          &GameDisplay::UpdateChannel);
  connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this,
          &GameDisplay::UpdateViews);

  // init display default page
  ui->stackedWidget->setCurrentIndex(2);
}

GameDisplay::~GameDisplay() { delete ui; }

void GameDisplay::UpdateChannel() {
  ui->channel_lay->ShowPageStuffs();
  ui->channel_lay->AddStuff();
}

void GameDisplay::UpdateViews(const QString &name) {
  ui->stackedWidget->setCurrentIndex(2);
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
  ui->stackedWidget->setCurrentIndex(0);
}

void GameDisplay::on_pushButton_2_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
}

void GameDisplay::on_stackedWidget_currentChanged(int arg1) {
  // ui->stackedWidget->
  if (arg1 == 0) {
    ui->attak_page->UpdateAttak("");
  }
}
