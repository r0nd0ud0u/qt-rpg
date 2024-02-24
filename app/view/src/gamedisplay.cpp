#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "actionsview.h"
#include "channel.h"
#include "heroesview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

  connect(ui->heroes_widget, &HeroesView::SigAddStuff, this,
          &GameDisplay::UpdateChannel);
  connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this,
          &GameDisplay::UpdateViews);
  connect(ui->channel_lay, &Channel::SigNextRound, this,
          &GameDisplay::NewRound);
  connect(ui->channel_lay, &Channel::SigNewTurn, this,
          &GameDisplay::StartNewTurn);
  connect(ui->channel_lay, &Channel::SigEndOfTurn, this,
          &GameDisplay::EndOfTurn);
  connect(ui->attak_page, &ActionsView::SigLaunchAttak, this,
          &GameDisplay::LaunchAttak);

  // init display default page
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
  // init status page
  ui->turn_label->setText("Tour 0");
}

GameDisplay::~GameDisplay() { delete ui; }

void GameDisplay::UpdateChannel() {
  ui->channel_lay->ShowPageStuffs();
  ui->channel_lay->AddStuff();
}

void GameDisplay::UpdateViews(const QString &name) {
  const auto &app = Application::GetInstance();
  // lambda function to add here
  for (auto *hero : app.m_GameManager->m_PlayersManager->m_HeroesList) {
    if (hero->m_Name == name) {
      app.m_GameManager->m_PlayersManager->m_SelectedHero = hero;
      break;
    }
  }
  emit selectCharacter(name);
}

void GameDisplay::on_attaque_button_clicked() {
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::attak));
  ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(true);
}

void GameDisplay::on_bag_button_clicked() {
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::inventory));
  ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(true);
}

void GameDisplay::on_stackedWidget_currentChanged(const int arg1) {
  ui->attak_page->UpdateActions(static_cast<ActionsStackedWgType>(arg1));
}

void GameDisplay::UpdateGameStatus() {
  const auto &gameState = Application::GetInstance().m_GameManager->m_GameState;
  ui->turn_label->setText(QString("Tour %1, Round %2/%3")
                              .arg(gameState->m_CurrentTurnNb)
                              .arg(gameState->m_CurrentRound)
                              .arg(gameState->m_OrderToPlay.size()));
}

void GameDisplay::NewRound() {
  const auto &gm = Application::GetInstance().m_GameManager;
  if (gm == nullptr) {
    return;
  }
  const auto &gs = gm->m_GameState;
  // TODO game state , check if boss is dead

  // First update the game state
  gs->m_CurrentRound++;
  UpdateGameStatus();

  // Get current player
  auto *activePlayer =
      Application::GetInstance().m_GameManager->GetCurrentPlayer();

  if (activePlayer == nullptr) {
    emit SigUpdateChannelView("Debug", "NewRound nullptr active player");
  }

  // Apply effects
  const QStringList effectsLogs = gm->m_PlayersManager->ApplyEffectsOnPlayer(
      activePlayer->m_Name, gm->m_GameState->m_CurrentTurnNb);
  for (const auto &el : effectsLogs) {
    emit SigUpdateChannelView("GameState", el);
  }
  // update effect
  const QStringList terminatedEffects =
      gm->m_PlayersManager->RemoveTerminatedEffectsOnPlayer(
          activePlayer->m_Name);
  for (const auto &te : terminatedEffects) {
    emit SigUpdateChannelView("GameState", te);
  }
  gm->m_PlayersManager->DecreaseCoolDownEffects(activePlayer->m_Name);
  emit SigUpdateAllEffectPanel(gm->m_PlayersManager->m_AllEffectsOnGame);
  // Updat views after stats changes
  emit SigUpdatePlayerPanel();

  // Update views
  // Players panels views
  ui->heroes_widget->ActivatePanel(activePlayer->m_Name);
  ui->bosses_widget->ActivatePanel(activePlayer->m_Name);
  // Activate actions buttons
  ui->bag_button->setEnabled(true);
  ui->attaque_button->setEnabled(true);
  // default page on action view
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
  // actions views
  ui->attak_page->SetCurrentPlayer(activePlayer);
  ui->inventory_page->SetCurrentPlayer(activePlayer);

  emit SigUpdateChannelView("GameState", QString("Round %1/%2")
                                             .arg(gs->m_CurrentRound)
                                             .arg(gs->m_OrderToPlay.size()));
  // TODO update channel
  // choice of talent
  // if dead -> choice to take a potion
}

void GameDisplay::StartNewTurn() {
  // TODO game state , check if boss is dead
  const auto &gm = Application::GetInstance().m_GameManager;

  // For each turn now
  // Process the order of the players
  gm->ProcessOrderToPlay(gm->m_GameState->m_OrderToPlay);
  // Update game state
  gm->m_GameState->m_CurrentRound = 0;
  gm->m_GameState->m_CurrentTurnNb++;
  emit SigUpdateChannelView(
      "GameState", QString("Tour %1").arg(gm->m_GameState->m_CurrentTurnNb));
  NewRound();
  // Then, update the display
  UpdateGameStatus();
  // game is just starting at turn 1
  // some first init to do for the views
  if (gm->m_GameState->m_CurrentTurnNb == 1) {
    ui->attak_page->InitTargetsWidget();
  }
  // Increment turn effects
  gm->m_PlayersManager->IncrementCounterEffect();
  // Apply regen stats
  gm->m_PlayersManager->ApplyRegenStats();
  // Updat views after stats changes
  emit SigUpdatePlayerPanel();
}

void GameDisplay::EndOfTurn() {
  ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(false);
  emit SigUpdateChannelView("GameState", "Fin du tour !!");
}

void GameDisplay::EndOfGame() {
  // Desactivate actions buttons
  ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(false);
  // default page on action view
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));

  emit SigUpdateChannelView("GameState", "Fin du jeu !!");
}

void GameDisplay::LaunchAttak(const QString &atkName,
                              const std::vector<TargetInfo> &targetList) {
  const auto &gm = Application::GetInstance().m_GameManager;

  // Desactivate actions buttons
  ui->bag_button->setEnabled(false);
  ui->attaque_button->setEnabled(false);
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));

  const auto &nameChara = gm->m_GameState->GetCurrentPlayerName();
  auto *activatedPlayer = gm->m_PlayersManager->GetCharacterByName(nameChara);
  if (activatedPlayer == nullptr) {
    return;
  }
  // launch atk
  if (activatedPlayer->m_AttakList.count(atkName) == 0) {
    return;
  }
  const auto &currentAtk = activatedPlayer->m_AttakList.at(atkName);
  // Stats change on hero
  activatedPlayer->ProcessCostAndRegen(atkName);
  emit SigUpdateChannelView(nameChara, QString("lance %1.").arg(atkName),
                            activatedPlayer->color);

  // new effects on that turn
  std::unordered_map<QString, std::vector<effectParam>> newEffects;
  // Parse target list and appliy atk and effects
  for (const auto &target : targetList) {
    QString channelLog;
    auto *targetChara = gm->m_PlayersManager->GetCharacterByName(target.m_Name);
    if (targetChara != nullptr) {
      // EFFECT
      const auto &[applyAtk, resultEffects, appliedEffects] =
          activatedPlayer->ApplyAtkEffect(target.m_IsTargeted, currentAtk,
                                          targetChara);
      if (!resultEffects.isEmpty()) {
        emit SigUpdateChannelView(nameChara,
                                  QString("Sur %1: ").arg(target.m_Name) +
                                      "\n" + resultEffects.join("\n"),
                                  activatedPlayer->color);
      }
      // applyAtk = false if effect reinit with unfulfilled condtions
      if (target.m_IsTargeted && applyAtk && !channelLog.isEmpty()) {
        // Update channel view
        emit SigUpdateChannelView(nameChara, channelLog,
                                  activatedPlayer->color);
      }
      // add applied effect to new effect Table
      newEffects[target.m_Name] = appliedEffects;
    }
  }

  /// Update game state
  // update effect on player manager
  for (const auto &[targetName, epTable] : newEffects) {
    if (epTable.empty()) {
      continue;
    }
    gm->m_PlayersManager->AddGameEffectOnAtk(activatedPlayer->m_Name,
                                             currentAtk, targetName, epTable,
                                             gm->m_GameState->m_CurrentTurnNb);
    // remove terminated effects
    // Some effects like "delete one bad effect" need to be updated
    const QStringList terminatedEffects =
        gm->m_PlayersManager->RemoveTerminatedEffectsOnPlayer(targetName);
    for (const auto &te : terminatedEffects) {
      emit SigUpdateChannelView(nameChara, te);
    }
  }
  // update all effect panel
  emit SigUpdateAllEffectPanel(gm->m_PlayersManager->m_AllEffectsOnGame);

  // update views of heroes and bosses
  emit SigUpdatePlayerPanel();

  // check who is dead!
  for (auto &boss : gm->m_PlayersManager->m_BossesList) {
    const auto &hp =
        std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_HP]);
    if (hp.m_CurrentValue == 0) {
      // next phase
      emit SigBossDead(boss->m_Name);
      // delete bosses in player manager
      delete boss;
      boss = nullptr;
    }
  }
  // Check end of game
  if (gm->m_PlayersManager->m_BossesList.empty()) {
    // update buttons
    EndOfGame();
  }

  uint8_t nbDeadHeroes = 0;
  for (const auto &hero : gm->m_PlayersManager->m_HeroesList) {
    const auto &hp =
        std::get<StatsType<int>>(hero->m_Stats.m_AllStatsTable[STATS_HP]);
    if (hp.m_CurrentValue == 0) {
      // choose to drink a potion
      nbDeadHeroes++;
    }
  }
  if (nbDeadHeroes == gm->m_PlayersManager->m_HeroesList.size()) {
    // end of game
  }
}
