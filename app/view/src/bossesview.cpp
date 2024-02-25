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
  InitBossPanels();

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdatePlayerPanel,
          this, &BossesView::UpdateAllPanels);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigBossDead, this,
          &BossesView::RemoveBoss);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigAddCharacter,
          this, &BossesView::AddBossPanel);
  connect((GameDisplay*)parentWidget(), &GameDisplay::SigSetFocusOnActivePlayer, this, &BossesView::SetFocusOn);
  connect((GameDisplay*)parentWidget(), &GameDisplay::selectCharacter, this, &BossesView::UpdateSelected);
}

BossesView::~BossesView() {
  for (auto *it : m_BossPanels) {
    delete it;
    it = nullptr;
  }
  m_BossPanels.clear();
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
  connect(bossPanel, &BossPanel::SigSelectedCharacterOnPanel, this,
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

void BossesView::UpdateAllPanels() {
  for (auto &panel : m_BossPanels) {
    panel->UpdatePanel(panel->m_Boss);
  }
}

void BossesView::RemoveBoss(QString bossName) {
  auto *lay = ui->main_widget->layout();
  int i = 0;
  for (auto &it : m_BossPanels) {
    if (it->m_Boss->m_Name == bossName) {
      lay->removeWidget(it);
      delete it;
      it = nullptr;
      i++;
      lay->removeItem(lay->itemAt(i - 1));
    }
  }
}

void BossesView::SetFocusOn(const QString& name, const characType& type){
    if(type != characType::Boss){
        return;
    }
    for(int i = 0; i< ui->main_widget->layout()->count(); i++){
        auto *wg = static_cast<BossPanel *>(
            ui->main_widget->layout()->itemAt(i)->widget());
        if(wg != nullptr && wg->m_Boss->m_Name == name){
            ui->scrollArea->ensureWidgetVisible(wg);
        }
    }
}

void BossesView::SlotClickedOnPanel(const QString& name){
    UpdateSelected(name);

    emit SigClickedOnPanel(name);
}

void BossesView::UpdateSelected(const QString& name) const{
    for (auto* panel : m_BossPanels) {
        if(panel == nullptr){
            continue;
        }
        if (panel->m_Boss->m_Name == name) {
            panel->SetSelected(true);
        } else {
            panel->SetSelected(false);
        }
    }
}
