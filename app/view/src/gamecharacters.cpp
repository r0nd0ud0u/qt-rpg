#include "gamecharacters.h"
#include "ui_gamecharacters.h"

#include "Application.h"

GameCharacters::GameCharacters(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameCharacters) {
  ui->setupUi(this);

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
  SetType(true);
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
  SetType(false);
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
      panel->SetSelectedGameChoiceBtn(true);
      ui->heroScrollAreaWidgetContents->layout()->addWidget(panel);
      connect(panel, &BossPanel::SigPanelSelectCharacter, this,
              &GameCharacters::SelectPanel);
      connect(panel, &BossPanel::SigUpdateCharacterPlaying, this,
              &GameCharacters::UpdateCharacterPlaying);
      panel->SetActive(false);
      m_BossesList.push_back(panel);
    }
  }
}

void GameCharacters::on_back_pushButton_clicked() {
  if (m_IsHeroType) {
    ResetAllCharacterPlaying();
  }
  emit SigBackBtnPushed(m_IsHeroType);
}

void GameCharacters::UpdateSelected(const Character* c) const {
  if (m_IsHeroType) {
    for (auto *hero : m_HeroesList) {
      if (hero == nullptr) {
        continue;
      }
      if (hero->m_Heroe->m_Name == c->m_Name) {
        hero->SetSelected(true);
      } else {
        hero->SetSelected(false);
      }
    }
  } else {
    for (auto *boss : m_BossesList) {
      if (boss == nullptr) {
        continue;
      }
      if (boss->m_Boss->m_Name == c->m_Name) {
        boss->SetSelected(true);
      } else {
        boss->SetSelected(false);
      }
    }
  }
}

void GameCharacters::SelectPanel(const Character* c) {
  UpdateSelected(c);
  emit SigSelectGameCharacter(c);
}

void GameCharacters::UpdateCharacterPlaying(const QString &name) const {
  if (m_IsHeroType) {
    for (auto *hero : m_HeroesList) {
      if (name == hero->m_Heroe->m_Name) {
        hero->m_Heroe->m_StatsInGame.StartGame(
            !hero->m_Heroe->m_StatsInGame.m_IsPlaying);
        break;
      }
    }
  } else {
    for (auto *boss : m_BossesList) {
      if (name == boss->m_Boss->m_Name) {
        boss->m_Boss->m_StatsInGame.StartGame(
            !boss->m_Boss->m_StatsInGame.m_IsPlaying);
        break;
      }
    }
  }
}

void GameCharacters::ResetAllCharacterPlaying() const {
  for (auto *hero : m_HeroesList) {
    hero->SetSelectStatus();
    hero->m_Heroe->m_StatsInGame.StartGame(false);
  }
  for (auto *boss : m_BossesList) {
    boss->SetSelectStatus();
    boss->m_Boss->m_StatsInGame.StartGame(false);
  }
}

void GameCharacters::on_nextPushButton_clicked() {
  emit SigNextButtonPushed(m_IsHeroType);
}

void GameCharacters::SetTextNextButton(const QString &str) {
  ui->nextPushButton->setText(str);
}

void GameCharacters::ResetPages() const { ResetAllCharacterPlaying(); }
