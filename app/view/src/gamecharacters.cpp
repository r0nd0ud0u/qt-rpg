#include "gamecharacters.h"
#include "ui_gamecharacters.h"

#include "Application.h"
#include "ApplicationView.h"

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
  if (const auto &app = Application::GetInstance();
      app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto &it :
         app.m_GameManager->m_PlayersManager->m_AllHeroesList) {
      if (it == nullptr) {
        continue;
      }
      InitHero(it);
    }
  }
  if (!m_HeroesList.empty()) {
    SelectPanel(m_HeroesList.front()->m_Heroe);
  }
}

void GameCharacters::InitAllBossesPanel() {
  SetType(false);
  m_BossesList.clear();
  if (const auto &app = Application::GetInstance();
      app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {
    for (const auto &it :
         app.m_GameManager->m_PlayersManager->m_AllBossesList) {
      if (it == nullptr) {
        continue;
      }
      InitBoss(it);
    }
  }
  if (!m_BossesList.empty()) {
    SelectPanel(m_BossesList.front()->m_Boss);
  }
}

void GameCharacters::on_back_pushButton_clicked() {
  if (m_IsHeroType) {
    ResetAllCharacterPlaying();
  }
  emit SigBackBtnPushed(m_IsHeroType);
}

void GameCharacters::UpdateSelected(const Character *c) const {
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

void GameCharacters::SelectPanel(const Character *c) {
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

void GameCharacters::on_addCharaPushButton_clicked() {
  AddCharacter();
  SetFocusLastOnPanel();
}

void GameCharacters::AddCharacter() {
  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr) {
    return;
  }
  auto *c = new Character();
  c->m_Name = app.m_GameManager->m_PlayersManager->ProcessNewDefaultName();
  if (m_IsHeroType) {
    InitHero(c);
    // in case of delete, responsqbility of m_PlayersManager
    app.m_GameManager->m_PlayersManager->m_AllHeroesList.push_back(c);
  } else {
    InitBoss(c);
    // in case of delete, responsqbility of m_PlayersManager
    app.m_GameManager->m_PlayersManager->m_AllBossesList.push_back(c);
  }
  // display edit character window
  auto &appView = ApplicationView::GetInstance();
  appView.GetCharacterWindow()->InitWindow(tabType::character, c);
  appView.ShowWindow(appView.GetCharacterWindow(), true);
}

void GameCharacters::InitHero(Character *c) {
  if (c == nullptr) {
    return;
  }
  c->m_type = characType::Hero;
  auto *panel = new HeroPanel();
  panel->SetPixmap(c->m_Name);
  panel->UpdatePanel(c, {});
  panel->SetSelectedGameChoiceBtn(true);
  connect(panel, &HeroPanel::SigPanelSelectCharacter, this,
          &GameCharacters::SelectPanel);
  connect(panel, &HeroPanel::SigUpdateCharacterPlaying, this,
          &GameCharacters::UpdateCharacterPlaying);
  connect(panel, &HeroPanel::SigRemovePanelByBtn, this,
          &GameCharacters::RemoveHeroPanel);
  panel->SetActive(false);
  m_HeroesList.push_back(panel);
  SelectPanel(panel->m_Heroe); // must be after m_HeroesList.push_back
  ui->heroScrollAreaWidgetContents->layout()->addWidget(panel);
  panel->show(); // useful i dont know why for the ensure focus
}

void GameCharacters::InitBoss(Character *c) {
  if (c == nullptr) {
    return;
  }
  // define as boss
  c->m_type = characType::Boss;
  auto *panel = new BossPanel();
  panel->UpdatePanel(c);
  panel->SetSelectedGameChoiceBtn(true);
  ui->heroScrollAreaWidgetContents->layout()->addWidget(panel);
  connect(panel, &BossPanel::SigPanelSelectCharacter, this,
          &GameCharacters::SelectPanel);
  connect(panel, &BossPanel::SigUpdateCharacterPlaying, this,
          &GameCharacters::UpdateCharacterPlaying);
  connect(panel, &BossPanel::SigRemovePanelByBtn, this,
          &GameCharacters::RemoveBossPanel);
  panel->SetActive(false);
  panel->SetSelected(false);
  m_BossesList.push_back(panel);
  SelectPanel(panel->m_Boss); // must be after m_BossesList.push_back
  panel->show();              // useful i dont know why for the ensure focus
}

void GameCharacters::SetFocusOnBossPanel(const Character *c) {
  if (c == nullptr) {
    return;
  }
  for (int i = 0; i < ui->heroScrollAreaWidgetContents->layout()->count();
       i++) {
    auto *wg = static_cast<BossPanel *>(
        ui->heroScrollAreaWidgetContents->layout()->itemAt(i)->widget());
    if (wg != nullptr && wg->m_Boss->m_Name == c->m_Name) {
      ui->bossScrollArea->ensureWidgetVisible(wg);
    }
  }
}

void GameCharacters::SetFocusOnHeroPanel(const Character *c) {
  if (c == nullptr) {
    return;
  }
  for (int i = 0; i < ui->heroScrollAreaWidgetContents->layout()->count();
       i++) {
    auto *wg = static_cast<HeroPanel *>(
        ui->heroScrollAreaWidgetContents->layout()->itemAt(i)->widget());
    if (wg != nullptr && wg->m_Heroe->m_Name == c->m_Name) {
      ui->bossScrollArea->ensureWidgetVisible(wg, 0, 0);
    }
  }
}

void GameCharacters::UpdatePanelAfterEdit(Character *c) {
  if (c == nullptr) {
    return;
  }
  SetFocusLastOnPanel();
  for (auto *pnl : m_HeroesList) {
    if (pnl != nullptr && pnl->m_Heroe != nullptr && pnl->m_Heroe == c) {
      pnl->UpdatePanel(c, {});
    }
  }
  for (auto *pnl : m_BossesList) {
    if (pnl != nullptr && pnl->m_Boss != nullptr && pnl->m_Boss == c) {
      pnl->UpdatePanel(c);
    }
  }
}

void GameCharacters::SetFocusLastOnPanel() {
  const auto nbWg = ui->heroScrollAreaWidgetContents->layout()->count();
  if (nbWg == 0) {
    return;
  }
  auto *wg = static_cast<HeroPanel *>(
      ui->heroScrollAreaWidgetContents->layout()->itemAt(nbWg - 1)->widget());
  if (wg != nullptr) {
    ui->bossScrollArea->ensureWidgetVisible(wg, 0, 0);
  }
}

void GameCharacters::RemoveHeroPanel(const HeroPanel *pnl) {
  if (pnl == nullptr) {
    return;
  }
  auto *lay = ui->heroScrollAreaWidgetContents->layout();
  if (lay == nullptr) {
    return;
  }
  // find index of panel before being erase of panel list
  int i = 0;
  for (const auto *it : m_HeroesList) {
    if (it == pnl) {
      // Remove file
      if (KERNEL_PLAYERS.count(it->m_Heroe->m_Name) == 0) {
        Utils::RemoveFile(OFFLINE_CHARACTERS + it->m_Heroe->m_Name + ".json");
        delete it->m_Heroe;
      }
      break;
    }
    i++;
  }

  // find iterator of panel
  const auto newEnd = std::remove_if(
      m_HeroesList.begin(), m_HeroesList.end(), [&](const HeroPanel *p) {
        return p == pnl; // remove elements where this is true
      });
  // erase panel of panel list
  m_HeroesList.erase(newEnd, m_HeroesList.end());

  // remove widget thanks to index
  auto *widget = lay->itemAt(i)->widget();
  widget->hide();
  lay->removeItem(lay->itemAt(i));
  lay->removeWidget(widget);
  delete widget;
}

void GameCharacters::RemoveBossPanel(const BossPanel *pnl) {
  if (pnl == nullptr) {
    return;
  }
  auto *lay = ui->heroScrollAreaWidgetContents->layout();
  if (lay == nullptr) {
    return;
  }
  // find index of panel before being erase of panel list
  int i = 0;
  for (const auto *it : m_BossesList) {
    if (it == pnl) {
      // Remove file
      Utils::RemoveFile(OFFLINE_CHARACTERS + it->m_Boss->m_Name + ".json");
      delete it->m_Boss;
      break;
    }
    i++;
  }

  // find iterator of panel
  const auto newEnd = std::remove_if(
      m_BossesList.begin(), m_BossesList.end(), [&](const BossPanel *p) {
        return p == pnl; // remove elements where this is true
      });
  // erase panel of panel list
  m_BossesList.erase(newEnd, m_BossesList.end());

  // remove widget thanks to index
  auto *widget = lay->itemAt(i)->widget();
  widget->hide();
  lay->removeItem(lay->itemAt(i));
  lay->removeWidget(widget);
  delete widget;
}
