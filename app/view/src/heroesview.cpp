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

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdatePlayerPanel,
          this, &HeroesView::UpdateAllPanels);
  connect((GameDisplay *)parentWidget(),
          &GameDisplay::SigSetFocusOnActivePlayer, this,
          &HeroesView::SetFocusOn);
  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &HeroesView::UpdateSelected);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigGameDisplayStart,
          this, &HeroesView::InitHeroPanel);
}

HeroesView::~HeroesView() {
  ResetUi();
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
      heroPanel->UpdatePanel(it, {});
      ui->left_widget->layout()->addWidget(heroPanel);
      m_HeroPanels.push_back(heroPanel);
      heroPanel->InitComboBox();
      heroPanel->SetActive(false);
      heroPanel->SetSelected(false);
      heroPanel->ProcessPlayingMode();
      connect(heroPanel, &HeroPanel::SigPanelSelectCharacter, this,
              &HeroesView::SlotClickedOnHeroPanel);
      connect(heroPanel, &HeroPanel::SigUpdateCharaForm, this,
              &HeroesView::SlotSelectedForm);
    }
    if (!m_HeroPanels.empty()) {
      m_HeroPanels.front()->SetActive(true);
      m_HeroPanels.front()->SetSelected(true);
    }
  }
}

void HeroesView::SlotClickedOnHeroPanel(const Character *c) {

  UpdateSelected(c);
  emit SigClickedOnHeroPanel(c->m_Name);
}

void HeroesView::SlotSelectedForm(const QString &playerName,
                                  const QString &form) {
  emit SigSelectedFormOnHeroPanel(playerName, form);
}

void HeroesView::UpdateSelected(const Character *c) const {
  if (c == nullptr) {
    return;
  }
  for (auto *hero : m_HeroPanels) {
    if (hero == nullptr) {
      continue;
    }
    if (hero->m_Heroe->m_Name == c->m_Name) {
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

void HeroesView::UpdateAllPanels(
    const std::unordered_map<QString, std::vector<GameAtkEffects>>
        &allPlEffects) {
  for (auto *heroPanel : m_HeroPanels) {
    if (heroPanel == nullptr) {
      continue;
    }
    const auto plEffects = (allPlEffects.count(heroPanel->m_Heroe->m_Name) > 0)
                               ? allPlEffects.at(heroPanel->m_Heroe->m_Name)
                               : std::vector<GameAtkEffects>();
    heroPanel->UpdatePanel(heroPanel->m_Heroe, plEffects);
  }
}

void HeroesView::SetFocusOn(const Character *c) {
    if (c->m_Type != characType::Hero) {
    return;
  }
  for (int i = 0; i < ui->left_widget->layout()->count(); i++) {
    auto *wg = static_cast<HeroPanel *>(
        ui->left_widget->layout()->itemAt(i)->widget());
    if (wg != nullptr && wg->m_Heroe->m_Name == c->m_Name) {
      ui->scrollArea->ensureWidgetVisible(wg);
    }
  }
}

void HeroesView::ResetUi() {
  for (auto *it : m_HeroPanels) {
    delete it;
    it = nullptr;
  }
  m_HeroPanels.clear();
}
