#include "heroesview.h"
#include "ui_heroesview.h"

#include "Application.h"

#include "gamedisplay.h"
#include "heropanel.h"

HeroesView::HeroesView(QWidget *parent)
    : QWidget(parent), ui(new Ui::HeroesView) {
  ui->setupUi(this);
  setStyleSheet(
      "#left_widget{ background:     #808080;} "
      "#right_widget{background:     #808080;} QLabel{color: white;}");
  InitHeroPanel();

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdatePlayerPanel,
          this, &HeroesView::UpdateAllPanels);
  connect((GameDisplay *)parentWidget(),
          &GameDisplay::SigSetFocusOnActivePlayer, this,
          &HeroesView::SetFocusOn);
  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &HeroesView::UpdateSelected);
}

HeroesView::~HeroesView() {
  for (auto *it : m_HeroPanels) {
    delete it;
    it = nullptr;
  }
  m_HeroPanels.clear();
  delete ui;
}

void HeroesView::InitHeroPanel() {

  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto &it : app.m_GameManager->m_PlayersManager->m_HeroesList) {
      if (it == nullptr) {
        continue;
      }
      auto *heroPanel = new HeroPanel();
      heroPanel->SetPixmap(it->m_Name);
      heroPanel->UpdatePanel(it);
      ui->left_widget->layout()->addWidget(heroPanel);
      m_HeroPanels.push_back(heroPanel);
      heroPanel->SetActive(false);
      heroPanel->SetSelected(false);
      connect(heroPanel, &HeroPanel::SigPanelSelectCharacter, this,
              &HeroesView::SlotClickedOnHeroPanel);
    }
    if (!m_HeroPanels.empty()) {
      m_HeroPanels.front()->SetActive(true);
      m_HeroPanels.front()->SetSelected(true);
    }
  }
}

void HeroesView::Dosomething() { emit SigAddStuff(); }

void HeroesView::SlotClickedOnHeroPanel(const QString &name) {

  UpdateSelected(name);
  emit SigClickedOnHeroPanel(name);
}

void HeroesView::UpdateSelected(const QString &name)  const{
  for (auto *hero : m_HeroPanels) {
    if (hero == nullptr) {
      continue;
    }
    if (hero->m_Heroe->m_Name == name) {
      hero->SetSelected(true);
    } else {
      hero->SetSelected(false);
    }
  }
}

void HeroesView::ActivatePanel(const QString &heroName) {
  for (auto *hero : m_HeroPanels) {
    if (hero == nullptr) {
      continue;
    }
    if (heroName == hero->m_Heroe->m_Name) {
      hero->SetActive(true);
    } else {
      hero->SetActive(false);
    }
  }
}

void HeroesView::UpdateAllPanels() {
  for (auto *heroPanel : m_HeroPanels) {
    if (heroPanel == nullptr) {
      continue;
    }
    heroPanel->UpdatePanel(heroPanel->m_Heroe);
  }
}

void HeroesView::SetFocusOn(const QString &name, const characType &type) {
  if (type != characType::Hero) {
    return;
  }
  for (int i = 0; i < ui->left_widget->layout()->count(); i++) {
    auto *wg = static_cast<HeroPanel *>(
        ui->left_widget->layout()->itemAt(i)->widget());
    if (wg != nullptr && wg->m_Heroe->m_Name == name) {
      ui->scrollArea->ensureWidgetVisible(wg);
    }
  }
}
