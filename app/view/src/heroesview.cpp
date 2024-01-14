#include "heroesview.h"
#include "ui_heroesview.h"

#include "Application.h"
#include "heropanel.h"

#include <memory>

HeroesView::HeroesView(QWidget *parent)
    : QWidget(parent), ui(new Ui::HeroesView) {
  ui->setupUi(this);

  InitHeroPanel();
}

HeroesView::~HeroesView() { delete ui; }

void HeroesView::InitHeroPanel() {

  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr && app.m_GameManager->m_Heroes != nullptr) {
    for (const auto &it : app.m_GameManager->m_Heroes->m_HeroesList) {
      if (it == nullptr) {
        continue;
      }
      auto heroPanel = std::make_shared<HeroPanel>();
      heroPanel->m_Heroe = it;
      ui->left_heroes_lay->addWidget(heroPanel.get());
      m_HeroesList.push_back(heroPanel.get());
      connect(heroPanel.get(), &HeroPanel::addStuff, this,
              &HeroesView::Dosomething);
    }
  }
}

void HeroesView::Dosomething() { emit SigAddStuff(); }
