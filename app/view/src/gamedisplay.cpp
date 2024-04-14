#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "ApplicationView.h"
#include "actionsview.h"
#include "bossesview.h"
#include "channel.h"
#include "heroesview.h"

#include "rust-rpg-bridge/utils.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

  connect(ui->heroes_widget, &HeroesView::SigAddStuff, this,
          &GameDisplay::UpdateChannel);
  connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this,
          &GameDisplay::UpdateViews);
  connect(ui->heroes_widget, &HeroesView::SigSelectedFormOnHeroPanel, this,
          &GameDisplay::SlotUpdateActionViews);
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

void GameDisplay::UpdateChannel() { ui->channel_lay->ShowPageStuffs(); }

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
  auto *gs = gm->m_GameState;
  if (gs == nullptr) {
    return;
  }
  gs->m_CurrentRound++;
  UpdateGameStatus();

  // Get current player
  auto *activePlayer = gm->GetCurrentPlayer();
  if (activePlayer == nullptr) {
    emit SigUpdateChannelView("Debug", "NewRound nullptr active player");
  }

  // Apply effects
  const QStringList effectsLogs = gm->m_PlayersManager->ApplyEffectsOnPlayer(
      activePlayer->m_Name, gm->m_GameState->m_CurrentTurnNb, false);
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
  emit SigUpdateAllEffectPanel(gm->m_PlayersManager->m_AllEffectsOnGame);

  // update buf pow
  if (activePlayer->m_Name == "Azrak Ombresang") {
    auto &localStat = activePlayer->m_Stats.m_AllStatsTable[STATS_POW_PHY];
    auto *phyBuf =
        activePlayer->m_AllBufs[static_cast<int>(BufTypes::powPhyBuf)];
    if (phyBuf != nullptr) {
      Character::SetStatsOnEffect(
          localStat, -phyBuf->get_value() + activePlayer->m_HealRxOnTurn, true,
          false, true);
      phyBuf->set_buffers(activePlayer->m_HealRxOnTurn,
                          phyBuf->get_is_percent());
    }
  }

  // reset heal received on turn
  activePlayer->m_HealRxOnTurn = 0;

  // process actions on last turn damage received
  const bool isDamageTxLastTurn =
      activePlayer->m_LastDamageTX.find(gs->m_CurrentTurnNb - 1) !=
      activePlayer->m_LastDamageTX.end();
  // passive power is_crit_heal_after_crit
  if (activePlayer->m_Power.is_crit_heal_after_crit && isDamageTxLastTurn &&
      activePlayer->m_isLastAtkCritical) {
    // in case of critical damage sent on last turn , next heal critical is
    // enable
    auto *buf =
        activePlayer->m_AllBufs[static_cast<int>(BufTypes::nextHealAtkIsCrit)];
    if (buf != nullptr) {
      buf->set_is_passive_enabled(true);
    }
  }
  // passive power
  if (activePlayer->m_Power.is_damage_tx_heal_needy_ally &&
      isDamageTxLastTurn) {
    gm->m_PlayersManager->ProcessDamageTXHealNeedyAlly(
        activePlayer->m_type,
        activePlayer->m_LastDamageTX[gs->m_CurrentTurnNb - 1]);
  }

  // Update views
  // Update views after stats changes
  // Players panels views
  ui->heroes_widget->ActivatePanel(activePlayer->m_Name);
  ui->bosses_widget->ActivatePanel(activePlayer->m_Name);
  // Activate actions buttons
  ui->bag_button->setEnabled(true);
  ui->attaque_button->setEnabled(true);
  // actions views
  ui->attak_page->SetCurrentPlayer(activePlayer);
  // set focus on active player
  emit SigSetFocusOnActivePlayer(activePlayer->m_Name, activePlayer->m_type);
  emit SigUpdatePlayerPanel();
  emit SigUpdateChannelView("GameState", QString("Round %1/%2")
                                             .arg(gs->m_CurrentRound)
                                             .arg(gs->m_OrderToPlay.size()));
  // auto select hero
  gm->m_PlayersManager->m_SelectedHero = activePlayer;
  emit selectCharacter(activePlayer->m_Name);

  // set atk view on
  // do it after hero has been activated and selected
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::attak));
  ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(true);
  ui->attak_page->UpdateActions(ActionsStackedWgType::attak);
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
  // Update views after stats changes
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

bool GameDisplay::ProcessAtk(
    const TargetInfo *target, const AttaqueType &currentAtk,
    Character *activatedPlayer, const bool isCrit, const QString &nameChara,
    std::unordered_map<QString, std::vector<effectParam>> &newEffects) {
  const auto &gm = Application::GetInstance().m_GameManager;
  if (target == nullptr) {
    return false;
  }
  QString channelLog;
  if (auto *targetChara =
          gm->m_PlayersManager->GetCharacterByName(target->get_name().data());
      targetChara != nullptr) {
    // is dodging
    if (currentAtk.target == TARGET_ENNEMY && currentAtk.reach == REACH_ZONE &&
        target->get_is_targeted()) {
      const auto &[isDodgingZone, outputsRandnbZone] = targetChara->IsDodging();
      if (isDodgingZone) {
        emit SigUpdateChannelView(
            targetChara->m_Name,
            QString("esquive.(%1)").arg(outputsRandnbZone));
        return false;
      } else {
        emit SigUpdateChannelView(
            targetChara->m_Name,
            QString("pas d'esquive.(%1)").arg(outputsRandnbZone));
      }
    }
    // EFFECT
    const auto &[conditionsOk, resultEffects, appliedEffects] =
        activatedPlayer->ApplyAtkEffect(target->get_is_targeted(), currentAtk,
                                        targetChara, isCrit);

    if (!resultEffects.isEmpty()) {
      emit SigUpdateChannelView(
          nameChara,
          QString("Sur %1: ").arg(target->get_name().data()) + "\n" +
              resultEffects.join(""),
          activatedPlayer->color);
    }
    // conditionsOk = false if effect reinit with unfulfilled condtions
    if (target->get_is_targeted() && conditionsOk && !channelLog.isEmpty()) {
      // Update channel view
      emit SigUpdateChannelView(nameChara, channelLog, activatedPlayer->color);
    }
    if (!conditionsOk) {
      ui->bag_button->setEnabled(true);
      ui->attaque_button->setEnabled(true);
      return false;
    }
    // add applied effect to new effect Table
    newEffects[target->get_name().data()] = appliedEffects;
  }

  return true;
}

/**
 * @brief GameDisplay::LaunchAttak
 * Atk of the launcher is processed first to enable the potential bufs
 * then the effets are processed on the other targets(ennemy and allies)
 */
void GameDisplay::LaunchAttak(const QString &atkName,
                              const std::vector<TargetInfo *> &targetList) {
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
  // Process cost of the atk
  activatedPlayer->ProcessCost(atkName);
  emit SigUpdateChannelView(nameChara, QString("lance %1.").arg(atkName),
                            activatedPlayer->color);

  // is Dodging
  if (currentAtk.target == TARGET_ENNEMY &&
      currentAtk.reach == REACH_INDIVIDUAL) {
    const auto &[isDodging, plName, outputsRandNb] =
        gm->m_PlayersManager->IsDodging(targetList);
    if (isDodging) {
      emit SigUpdateChannelView(
          plName, QString("esquive.(%1)").arg(outputsRandNb.first()));
      return;
    } else {
      emit SigUpdateChannelView(
          plName, QString("pas d'esquive.(%1)").arg(outputsRandNb.first()));
    }
  }

  // is critical Strike ??
  const auto [isCrit, critRandNb] =
      activatedPlayer->ProcessCriticalStrike(currentAtk);
  QString critStr;
  if (isCrit) {
    critStr = "Coup Critique";
  } else {
    critStr = "pas de coup critique";
  }
  emit SigUpdateChannelView(
      nameChara,
      QString("Test coup critique:%1 -> %2.\n").arg(critRandNb).arg(critStr));

  // In case of effect with reach: REACH_RAND_INDIVIDUAL, process who is the
  // random target
  gm->m_PlayersManager->ProcessIsRandomTarget();

  // new effects on that turn
  std::unordered_map<QString, std::vector<effectParam>> newEffects;
  // Process first the buf of the launcher
  auto it = std::find_if(targetList.begin(), targetList.end(),
                         [&](const TargetInfo *ti) {
                           if (ti != nullptr) {
                             return nameChara == ti->get_name().data();
                           }
                           return false;
                         });
  if (it != targetList.end()) {
    ProcessAtk(*it, currentAtk, activatedPlayer, isCrit, nameChara, newEffects);
  }
  for (const auto *target : targetList) {
    if (target->get_name().data() == nameChara) {
      continue;
    }
    if (!ProcessAtk(target, currentAtk, activatedPlayer, isCrit, nameChara,
                    newEffects)) {
      return;
    }
  }

  // end of activated bufs during turn
  activatedPlayer->ResetBuf(BufTypes::damageCritCapped);
  activatedPlayer->ResetBuf(BufTypes::multiValue);
  activatedPlayer->ResetBuf(BufTypes::applyEffectInit);

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
    // TODO what to do when a boss is dead
    emit SigBossDead(dp);
    ui->attak_page->RemoveTarget(dp);
    ui->add_exp_button->setEnabled(true);
    gm->m_GameState->RemoveDeadPlayerInTurn(dp);
    gm->m_GameState->m_DiedEnnemies[gm->m_GameState->m_CurrentTurnNb].push_back(
        dp);
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

void GameDisplay::AddNewStuff() const {
  // TODO useful ?
  // emit
}

void GameDisplay::on_mana_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_MANA);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_hp_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_HP);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_berseck_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_BERSECK);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_vigor_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_VIGOR);
    emit SigUpdatePlayerPanel();
  }
}

void GameDisplay::on_add_exp_button_clicked() {
  Application::GetInstance().m_GameManager->m_PlayersManager->AddExpForHeroes(
      ui->exp_spinBox->value());
  // update level + exp label of each hero panel
  emit SigUpdatePlayerPanel();
  emit selectCharacter(
      Application::GetInstance()
          .m_GameManager->m_PlayersManager->m_SelectedHero->m_Name);
}

void GameDisplay::SlotUpdateActionViews(const QString &name,
                                        const QString &form) {
  const auto &gm = Application::GetInstance().m_GameManager;
  const auto &nameChara = gm->m_GameState->GetCurrentPlayerName();
  if (nameChara != name) {
    return;
  }
  auto *activatedPlayer = gm->m_PlayersManager->GetCharacterByName(nameChara);
  if (activatedPlayer == nullptr) {
    return;
  }
  if (name == "Thalia") {
    activatedPlayer->SetValuesForThalia(form == BEAR_FORM);
    emit SigUpdatePlayerPanel();
  }

  // update form
  ui->attak_page->SetForm(form);
  // reset the attak view
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::attak));
  ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(true);
  ui->attak_page->UpdateActions(ActionsStackedWgType::attak);
}
