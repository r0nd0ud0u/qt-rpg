#include "heroesview.h"
#include "ui_heroesview.h"

#include "Application.h"

#include "heropanel.h"

HeroesView::HeroesView(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::HeroesView)
{
  ui->setupUi(this);
  setStyleSheet("#left_widget{ background:     #808080;} "
                "#right_widget{background:     #808080;}");
  InitHeroPanel();


}

HeroesView::~HeroesView()
{
  for (auto* it : m_HeroPanels) {
    delete it;
    it = nullptr;
  }
  m_HeroPanels.clear();
  delete ui;
}

void
HeroesView::InitHeroPanel()
{

  const auto& app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto& it : app.m_GameManager->m_PlayersManager->m_HeroesList) {
      if (it == nullptr) {
        continue;
      }
      auto* heroPanel = new HeroPanel();
      heroPanel->UpdatePanel(it);
      ui->left_widget->layout()->addWidget(heroPanel);
      m_HeroPanels.push_back(heroPanel);
      heroPanel->SetActive(false);
      connect(heroPanel, &HeroPanel::addStuff, this, &HeroesView::Dosomething);
      connect(heroPanel, &HeroPanel::selectCharacter, this, &HeroesView::SlotClickedOnHeroPanel);
    }
    if (!m_HeroPanels.empty()) {
      m_HeroPanels.front()->SetActive(true);
    }
  }
}

void
HeroesView::Dosomething()
{
  emit SigAddStuff();
}

void
HeroesView::SlotClickedOnHeroPanel(QString name)
{
    emit SigClickedOnHeroPanel(name);
}

