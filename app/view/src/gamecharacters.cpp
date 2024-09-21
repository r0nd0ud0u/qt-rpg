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
  for (auto *it : m_BossesList) {
    delete it;
    it = nullptr;
  }
  m_BossesList.clear();
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
      heroPanel->SetSelectedGameChoiceBtn(true);
      ui->heroScrollAreaWidgetContents->layout()->addWidget(heroPanel);
      connect(heroPanel, &HeroPanel::SigPanelSelectCharacter, this,
              &GameCharacters::SelectPanel);
      connect(heroPanel, &HeroPanel::SigUpdateCharacterPlaying, this,
              &GameCharacters::UpdateCharacterPlaying);
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
      // auto *panel = new BossPanel();
      // panel->UpdatePanel(it);
      // ui->bossScrollAreaWidgetContents->layout()->addWidget(panel);
      // connect(panel, &BossPanel::SigSelectedCharacterOnPanel, this,
      //         &GameCharacters::ActivatePanel);
      // panel->SetActive(false);
      // m_BossesList.push_back(panel);
    }
  }
}

void GameCharacters::on_back_pushButton_clicked() {
  m_HeroesList.clear();
  m_BossesList.clear();
  emit SigReturnToHostPage();
}

void GameCharacters::UpdateSelected(const QString &name) const {
  for (auto *hero : m_HeroesList) {
    if (hero == nullptr) {
      continue;
    }
    if (hero->m_Heroe->m_Name == name) {
      hero->SetSelected(true);
    } else {
      hero->SetSelected(false);
    }
  }
  for (auto *c : m_BossesList) {
    if (c == nullptr) {
      continue;
    }
    if (c->m_Boss->m_Name == name) {
      c->SetSelected(true);
    } else {
      c->SetSelected(false);
    }
  }
}

void GameCharacters::SelectPanel(const QString &name) {
  UpdateSelected(name);
  for (const auto *hero : m_HeroesList) {
    if (name == hero->m_Heroe->m_Name) {
      emit SigSelectGameCharacter(hero->m_Heroe->m_Name,
                                  hero->m_Heroe->GetPhotoName());
      break;
    }
  }
  for (const auto *boss : m_BossesList) {
    if (name == boss->m_Boss->m_Name) {
      emit SigSelectGameCharacter(boss->m_Boss->m_Name,
                                  boss->m_Boss->GetPhotoName());
      break;
    }
  }
}

void GameCharacters::UpdateCharacterPlaying(const QString &name) const{
  for (auto *hero : m_HeroesList) {
    if (name == hero->m_Heroe->m_Name) {
      hero->m_Heroe->m_StatsInGame.StartGame(
          !hero->m_Heroe->m_StatsInGame.m_IsPlaying);
      break;
    }
  }
  for (auto *boss : m_BossesList) {
    if (name == boss->m_Boss->m_Name) {
      boss->m_Boss->m_StatsInGame.StartGame(
          !boss->m_Boss->m_StatsInGame.m_IsPlaying);
      break;
    }
  }
}
