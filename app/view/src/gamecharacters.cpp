#include "gamecharacters.h"
#include "ui_gamecharacters.h"

#include "Application.h"

GameCharacters::GameCharacters(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameCharacters) {
  ui->setupUi(this);

  InitAllHeroesPanel();
  InitAllBossesPanel();
  setStyleSheet("#bossScrollAreaWidgetContents{ background:     #000000; "
                "QLabel{color: white;}} "
                "#heroScrollAreaWidgetContents{background:     #808080;} "
                "QLabel{color: white;}");
}

GameCharacters::~GameCharacters() {
  for (auto *it : m_HeroesList) {
    delete it;
    it = nullptr;
  }
  m_HeroesList.clear();
  delete ui;
}

void GameCharacters::InitAllHeroesPanel() {

  m_HeroesList.clear();
  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto &it :
         app.m_GameManager->m_PlayersManager->m_AllHeroesList) {
      if (it == nullptr) {
        continue;
      }
      auto *heroPanel = new HeroPanel();
      heroPanel->SetPixmap(it->m_Name);
      heroPanel->UpdatePanel(it, {});
      ui->heroScrollAreaWidgetContents->layout()->addWidget(heroPanel);
      connect(heroPanel, &HeroPanel::SigPanelSelectCharacter, this,
              &GameCharacters::ActivatePanel);
      heroPanel->SetActive(false);
      m_HeroesList.push_back(heroPanel);
    }
  }
}

void GameCharacters::InitAllBossesPanel() {
  m_BossesList.clear();
  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto &it :
         app.m_GameManager->m_PlayersManager->m_AllBossesList) {
      if (it == nullptr) {
        continue;
      }
      auto *panel = new BossPanel();
      panel->UpdatePanel(it);
      ui->bossScrollAreaWidgetContents->layout()->addWidget(panel);
      connect(panel, &BossPanel::SigSelectedCharacterOnPanel, this,
              &GameCharacters::ActivatePanel);
      panel->SetActive(false);
      m_BossesList.push_back(panel);
    }
  }
}

void GameCharacters::on_back_pushButton_clicked() {
  std::set<QString> activeCh;
  std::for_each(m_HeroesList.begin(), m_HeroesList.end(),
                [&](const HeroPanel *hp) {
                  if (hp != nullptr && hp->GetActive()) {
                    activeCh.insert(hp->m_Heroe->m_Name);
                    return;
                  }
                });
  std::for_each(m_BossesList.begin(), m_BossesList.end(),
                [&](const BossPanel *bp) {
                  if (bp != nullptr && bp->GetActive()) {
                    activeCh.insert(bp->m_Boss->m_Name);
                    return;
                  }
                });
  emit SigBackToHostPage(activeCh);
}

void GameCharacters::ActivatePanel(const QString &name) const {
  for (auto *hero : m_HeroesList) {
    if (name == hero->m_Heroe->m_Name) {
      hero->SetActive(!hero->GetActive());
    }
  }
  for (auto *boss : m_BossesList) {
    if (name == boss->m_Boss->m_Name) {
      boss->SetActive(!boss->GetActive());
    }
  }
}
