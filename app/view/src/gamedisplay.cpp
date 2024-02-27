#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "ApplicationView.h"
#include "actionsview.h"
#include "bossesview.h"
#include "channel.h"
#include "heroesview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

  connect(ui->heroes_widget, &HeroesView::SigAddStuff, this,
          &GameDisplay::UpdateChannel);
  connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this,
          &GameDisplay::UpdateViews);
  connect(ui->bosses_widget, &BossesView::SigClickedOnPanel, this,
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
  for (auto *boss : app.m_GameManager->m_PlayersManager->m_BossesList) {
    if (boss->m_Name == name) {
      app.m_GameManager->m_PlayersManager->m_SelectedHero = boss;
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
  if (gm.get() == nullptr) {
    return;
  }

  // First update the game state
  const auto &gs = gm->m_GameState;
  gs->m_CurrentRound++;
  UpdateGameStatus();

  // Get current player
  auto *activePlayer = gm->GetCurrentPlayer();
  if (activePlayer == nullptr) {
    emit SigUpdateChannelView("Debug", "NewRound nullptr active player");
  }

  // Apply effects
  const QStringList effectsLogs = gm->m_PlayersManager->ApplyEffectsOnPlayer(
      activePlayer->m_Name, gm->m_GameState->m_CurrentTurnNb);
  for (const auto &el : effectsLogs) {
    emit SigUpdateChannelView("GameState", el);
  }
  // Update effect
  const QStringList terminatedEffects =
      gm->m_PlayersManager->RemoveTerminatedEffectsOnPlayer(
          activePlayer->m_Name);
  for (const auto &te : terminatedEffects) {
    emit SigUpdateChannelView("GameState", te);
  }
  gm->m_PlayersManager->DecreaseCoolDownEffects(activePlayer->m_Name);
  emit SigUpdateAllEffectPanel(gm->m_PlayersManager->m_AllEffectsOnGame);

  // Update views
  // Update views after stats changes
  emit SigUpdatePlayerPanel();
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
  // set focus on active player
  emit SigSetFocusOnActivePlayer(activePlayer->m_Name, activePlayer->m_type);

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

  // Increment turn effects
  gm->m_PlayersManager->IncrementCounterEffect();
  // Apply regen stats
  gm->m_PlayersManager->ApplyRegenStats(characType::Boss);
  gm->m_PlayersManager->ApplyRegenStats(characType::Hero);
  // Updat views after stats changes
  emit SigUpdatePlayerPanel();

  // For each turn now
  // Process the order of the players
  gm->ProcessOrderToPlay(gm->m_GameState->m_OrderToPlay);
  emit SigUpdateChannelView("GameState", gm->ProcessLogOrderToPlay());
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
  emit SigBossDead("");
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
  activatedPlayer->ProcessCost(atkName);
  emit SigUpdateChannelView(nameChara, QString("lance %1.").arg(atkName),
                            activatedPlayer->color);

  // is Dodging
  if (currentAtk.target == TARGET_ENNEMY &&
      currentAtk.reach == REACH_INDIVIDUAL) {
    const auto &[isDodging, plName] =
        gm->m_PlayersManager->IsDodging(targetList);
    if (isDodging) {
      emit SigUpdateChannelView(plName, QString("esquive."));
      return;
    }
  }

  // new effects on that turn
  std::unordered_map<QString, std::vector<effectParam>> newEffects;
  // Parse target list and apply atk and effects
  for (const auto &target : targetList) {
    QString channelLog;
    auto *targetChara = gm->m_PlayersManager->GetCharacterByName(target.m_Name);
    if (targetChara != nullptr) {
      // is dodging
      if (currentAtk.target == TARGET_ENNEMY &&
          currentAtk.reach == REACH_ZONE) {
        const auto &[isDodging, plName] =
            gm->m_PlayersManager->IsDodging(targetList);
        if (isDodging) {
          emit SigUpdateChannelView(plName, QString("esquive."));
          continue;
        }
      }
      // EFFECT
      const auto &[conditionsOk, resultEffects, appliedEffects] =
          activatedPlayer->ApplyAtkEffect(target.m_IsTargeted, currentAtk,
                                          targetChara);

      if (!resultEffects.isEmpty()) {
        emit SigUpdateChannelView(nameChara,
                                  QString("Sur %1: ").arg(target.m_Name) +
                                      "\n" + resultEffects.join("\n"),
                                  activatedPlayer->color);
      }
      // conditionsOk = false if effect reinit with unfulfilled condtions
      if (target.m_IsTargeted && conditionsOk && !channelLog.isEmpty()) {
        // Update channel view
        emit SigUpdateChannelView(nameChara, channelLog,
                                  activatedPlayer->color);
      }
      if (!conditionsOk) {
        ui->bag_button->setEnabled(true);
        ui->attaque_button->setEnabled(true);
        return;
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
  // update stats view
  emit SigUpdStatsOnSelCharacter();

  // check who is dead!
  const QStringList diedBossList =
      gm->m_PlayersManager->CheckDiedPlayers(characType::Boss);
  for (const auto &dp : diedBossList) {
    emit SigUpdateChannelView(dp, "est mort.");
    emit SigBossDead(dp);
    ui->add_exp_button->setEnabled(true);
  }
  const QStringList diedHeroesList =
      gm->m_PlayersManager->CheckDiedPlayers(characType::Hero);
  for (const auto &dp : diedHeroesList) {
    emit SigUpdateChannelView(dp, "est mort.");
  }

  // Check end of game
  if (gm->m_PlayersManager->m_BossesList.empty()) {
    // update buttons
    // TODO new boss to add ? or new phase ?
    // if yes -> start new turn with new boss
    //      if not end of game!!

    EndOfGame();
  }
}

void GameDisplay::on_add_boss_button_clicked() {
  auto &appView = ApplicationView::GetInstance();
    appView.GetCharacterWindow()->InitWindow(tabType::character);
  appView.ShowWindow(appView.GetCharacterWindow(), true);
}

void GameDisplay::AddNewCharacter(Character *ch) {
  emit SigAddCharacter(ch);
  Application::GetInstance()
      .m_GameManager->m_PlayersManager->m_BossesList.push_back(ch);
}

void GameDisplay::AddNewStuff(){
    //emit
}

void GameDisplay::on_mana_potion_button_clicked() {
  auto *hero = Application::GetInstance()
                   .m_GameManager->m_PlayersManager->m_ActivePlayer;
  if (hero != nullptr) {
    hero->UsePotion(STATS_MANA);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_hp_potion_button_clicked() {
  auto *hero = Application::GetInstance()
                   .m_GameManager->m_PlayersManager->m_ActivePlayer;
  if (hero != nullptr) {
    hero->UsePotion(STATS_HP);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_berseck_potion_button_clicked() {
  auto *hero = Application::GetInstance()
                   .m_GameManager->m_PlayersManager->m_ActivePlayer;
  if (hero != nullptr) {
    hero->UsePotion(STATS_BERSECK);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_vigor_potion_button_clicked() {
  auto *hero = Application::GetInstance()
                   .m_GameManager->m_PlayersManager->m_ActivePlayer;
  if (hero != nullptr) {
    hero->UsePotion(STATS_VIGOR);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_pushButton_clicked()
{
    Application::GetInstance()
        .m_GameManager->m_PlayersManager->AddExpForHeroes(ui->exp_spinBox->value());
    ui->add_exp_button->setEnabled(false);
}

