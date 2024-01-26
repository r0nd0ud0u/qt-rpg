#include "bossesview.h"
#include "ui_bossesview.h"

#include "Application.h"
#include "bosspanel.h"

BossesView::BossesView(QWidget *parent)
    : QWidget(parent), ui(new Ui::BossesView) {
  ui->setupUi(this);
  setStyleSheet(
      "#main_widget{ background:     #000000;} QLabel{color: white;} ");
  InitBossPanels();
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
      if (it == nullptr) {
        continue;
      }
      auto *bossPanel = new BossPanel();
      bossPanel->UpdatePanel(it);
      ui->main_widget->layout()->addWidget(bossPanel);
      m_BossPanels.push_back(bossPanel);
      bossPanel->SetActive(false);
    }
  }
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
