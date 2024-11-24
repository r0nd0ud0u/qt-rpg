#include "bossesview.h"
#include "ui_bossesview.h"

#include "Application.h"
#include "bosspanel.h"
#include "gamedisplay.h"

BossesView::BossesView(QWidget *parent)
    : QWidget(parent), ui(new Ui::BossesView) {
  ui->setupUi(this);
  setStyleSheet(
      "#main_widget{ background:     #000000;} QLabel{color: white;}");

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdatePlayerPanel,
          this, &BossesView::UpdateAllPanels);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigBossDead, this,
          &BossesView::RemoveBoss);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigAddCharacter, this,
          &BossesView::AddBossPanel);
  connect((GameDisplay *)parentWidget(),
          &GameDisplay::SigSetFocusOnActivePlayer, this,
          &BossesView::SetFocusOn);
  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &BossesView::UpdateSelected);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigGameDisplayStart,
          this, &BossesView::InitBossPanels);
}

BossesView::~BossesView() {
  ResetUi();
  delete ui;
}

void BossesView::InitBossPanels() {
  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto &it : app.m_GameManager->m_PlayersManager->m_BossesList) {
      AddBossPanel(it);
    }
  }
}

void BossesView::AddBossPanel(Character *ch) {
  if (ch == nullptr) {
    return;
  }
  auto *bossPanel = new BossPanel();
  bossPanel->UpdatePanel(ch);
  ui->main_widget->layout()->addWidget(bossPanel);
  m_BossPanels.push_back(bossPanel);
  bossPanel->SetActive(false);
  bossPanel->SetSelected(false);
  bossPanel->ProcessPlayingMode();
  connect(bossPanel, &BossPanel::SigPanelSelectCharacter, this,
          &BossesView::SlotClickedOnPanel);
}

void BossesView::ActivatePanel(const QString &bossName) {
  for (auto *boss : m_BossPanels) {
    if (bossName == boss->m_Boss->m_Name) {
      boss->SetActive(true);
    } else {
      boss->SetActive(false);
    }
  }
}

void BossesView::UpdateAllPanels([[maybe_unused]] const std::unordered_map<
                                 QString, std::vector<GameAtkEffects>> &table) {
  for (auto &panel : m_BossPanels) {
    panel->UpdatePanel(panel->m_Boss);
  }
}

void BossesView::RemoveBoss(QString bossName) {
  if (bossName.isEmpty()) {
    return;
  }
  auto *lay = ui->main_widget->layout();

  int i = 0;
  for (const auto *it : m_BossPanels) {
    if (it->m_Boss->m_Name == bossName) {
      break;
    }
    i++;
  }

  const auto newEnd = std::remove_if(
      m_BossPanels.begin(), m_BossPanels.end(),
      [&bossName](const BossPanel *bp) {
        if (bp == nullptr || bp->m_Boss == nullptr) {
          return false;
        }
        return bossName ==
               bp->m_Boss->m_Name; // remove elements where this is true
      });
  m_BossPanels.erase(newEnd, m_BossPanels.end());
  auto *widget = lay->itemAt(i)->widget();
  widget->hide();
  lay->removeItem(lay->itemAt(i));
  lay->removeWidget(widget);
  delete widget;
}

void BossesView::SetFocusOn(const Character *c) {
    if (c->m_Type != characType::Boss) {
    return;
  }
  for (int i = 0; i < ui->main_widget->layout()->count(); i++) {
    auto *wg = static_cast<BossPanel *>(
        ui->main_widget->layout()->itemAt(i)->widget());
    if (wg != nullptr && wg->m_Boss->m_Name == c->m_Name) {
      ui->scrollArea->ensureWidgetVisible(wg);
    }
  }
}

void BossesView::SlotClickedOnPanel(const Character *c) {
  UpdateSelected(c);
  emit SigClickedOnPanel(c->m_Name);
}

void BossesView::UpdateSelected(const Character *c) const {
  if (c == nullptr) {
    return;
  }
  for (auto *panel : m_BossPanels) {
    if (panel == nullptr) {
      continue;
    }
    if (panel->m_Boss->m_Name == c->m_Name) {
      panel->SetSelected(true);
    } else {
      panel->SetSelected(false);
    }
  }
}

void BossesView::ResetUi() {
  for (auto *it : m_BossPanels) {
    delete it;
    it = nullptr;
  }
  m_BossPanels.clear();
}
