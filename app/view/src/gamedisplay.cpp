#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "Application.h"
#include "ApplicationView.h"
#include "actionsview.h"
#include "bossesview.h"
#include "channel.h"
#include "fightsoundlog.h"
#include "heroesview.h"

#include "statsingame.h"
#include "stuff.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameDisplay) {
  ui->setupUi(this);

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
  connect(this, &GameDisplay::SigBossDead, ui->attak_page,
          &ActionsView::RemoveTarget);
  // img view
  connect(this, &GameDisplay::selectCharacter, ui->img_char_widget,
          &ImgCharacterView::UpdateView);
  // stats view
  connect(this, &GameDisplay::selectCharacter, ui->stats_character,
          &StatsView::UpdateDisplayedCharStats);
  connect(this, &GameDisplay::SigUpdStatsOnSelCharacter, ui->stats_character,
          &StatsView::UpdateDisplayedCharStats);
  // equip view
  connect(this, &GameDisplay::selectCharacter, ui->equipment_widget,
          &EquipmentView::UpdateEquipment);
  // init status page
  ui->turn_label->setText("Tour 0");
}

GameDisplay::~GameDisplay() { delete ui; }

void GameDisplay::UpdateViews(const QString &name) {
  const auto &app = Application::GetInstance();
  app.m_GameManager->m_PlayersManager->SetSelectedPlayer(name);
  emit selectCharacter(app.m_GameManager->m_PlayersManager->m_SelectedPlayer);
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

void GameDisplay::UpdateGameStatus(const GameState *gs) {
  if (gs == nullptr) {
    ui->turn_label->setText("no game state");
    return;
  }
  ui->turn_label->setText(QString("Tour %1, Round %2/%3")
                              .arg(gs->m_CurrentTurnNb)
                              .arg(gs->m_CurrentRound)
                              .arg(gs->m_OrderToPlay.size()));
}

bool GameDisplay::NewRound() {
  auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm == nullptr) {
    return false;
  }
  // First update the game state
  auto *gs = gm->m_GameState;
  if (gs == nullptr) {
    return false;
  }
  gs->m_CurrentRound++;
  UpdateGameStatus(gs);

  // Get current player
  auto *activePlayer = gm->GetCurrentPlayer();
  if (activePlayer == nullptr) {
    emit SigUpdateChannelView("Debug", "NewRound nullptr active player");
    return false;
  }
  // The player can play the round only if alive
  if (activePlayer->IsDead()) {
    emit SigUpdateChannelView(activePlayer->m_Name, "est mort.",
                              activePlayer->color);
      if (activePlayer->m_Type == characType::Hero) {
        m_FightSound.AddToQueue(PLAYER_DEATH_SOUND);
    }
    return false;
  }
  // update character
  activePlayer->m_ActionsDoneInRound = 0;
  // Apply effects
  // Assess first round for the player
  // TODO create a method to do only on first round
  if (activePlayer->m_ExtCharacter != nullptr &&
      activePlayer->m_ExtCharacter->get_is_first_round()) {
    // update boolean
    activePlayer->m_ExtCharacter->set_is_first_round(false);

    // init aggro
    activePlayer->InitAggroOnTurn(gs->m_CurrentTurnNb);
    // Remove terminated effect
    const QStringList terminatedEffects =
        gm->m_PlayersManager->RemoveTerminatedEffectsOnPlayer(
            activePlayer->m_Name);
    emit SigUpdateChannelView(activePlayer->m_Name,
                              terminatedEffects.join("\n"),
                              activePlayer->color);

    // Update effect
    const QStringList effectsLogs = gm->m_PlayersManager->ApplyEffectsOnPlayer(
        activePlayer->m_Name, gm->m_GameState->m_CurrentTurnNb, false);
    emit SigUpdateChannelView(activePlayer->m_Name, effectsLogs.join("\n"),
                              activePlayer->color);
    // update buf pow
    // passive azrak TODO extract in a function
    if (activePlayer->m_Name == "Azrak Ombresang") {
      auto &localStat = activePlayer->m_Stats.m_AllStatsTable[STATS_POW_PHY];
      auto *phyBuf =
          activePlayer->m_AllBufs[static_cast<int>(BufTypes::powPhyBuf)];
      if (phyBuf != nullptr) {
        const auto &hpRxTable =
            activePlayer->m_LastTxRx[static_cast<int>(amountType::overHealRx)];
        int64_t hpRx = 0;
        if (hpRxTable.find(gs->m_CurrentTurnNb - 1) != hpRxTable.end()) {
          hpRx = hpRxTable.at(gs->m_CurrentTurnNb - 1);
        }
        // -phyBuf->get_value() : buf previous turn
        // hpRx : buf new turn
        QStringList azrakOverHeal;
        if (static_cast<int>(gs->m_CurrentTurnNb) - 2 > 0) {
          azrakOverHeal.append(QString("Overheal: Tour%1: -%2")
                                   .arg(gs->m_CurrentTurnNb - 2)
                                   .arg(phyBuf->get_value()));
        }
        azrakOverHeal.append(QString("Overheal: Tour%1: +%2")
                                 .arg(gs->m_CurrentTurnNb - 1)
                                 .arg(hpRx));
        azrakOverHeal.append(QString("Overheal total: Tour%1: %2")
                                 .arg(gs->m_CurrentTurnNb - 1)
                                 .arg(hpRx - phyBuf->get_value()));
        emit SigUpdateChannelView(activePlayer->m_Name,
                                  azrakOverHeal.join("\n"),
                                  activePlayer->color);
        const auto addValue = static_cast<int>(-phyBuf->get_value() + hpRx);
        Character::SetStatsOnEffect(localStat, addValue, false, true);
        phyBuf->set_buffers(hpRx, phyBuf->get_is_percent());
      }
    }

    // process actions on last turn damage received
    const auto &damageTx =
        activePlayer->m_LastTxRx[static_cast<int>(amountType::damageTx)];
    const bool isDamageTxLastTurn =
        damageTx.find(gs->m_CurrentTurnNb - 1) != damageTx.end();
    // passive power is_crit_heal_after_crit
    if (activePlayer->m_Power.is_crit_heal_after_crit && isDamageTxLastTurn &&
        activePlayer->m_isLastAtkCritical) {
      // in case of critical damage sent on last turn , next heal critical is
      // enable
      auto *buf =
          activePlayer
              ->m_AllBufs[static_cast<int>(BufTypes::nextHealAtkIsCrit)];
      if (buf != nullptr) {
        buf->set_is_passive_enabled(true);
      }
    }
    // passive power
    if (activePlayer->m_Power.is_damage_tx_heal_needy_ally &&
        isDamageTxLastTurn) {
      gm->m_PlayersManager->ProcessDamageTXHealNeedyAlly(
            activePlayer->m_Type, damageTx.at(gs->m_CurrentTurnNb - 1));
    }
  }

  // Assess boss atk
  if (const auto randAtkNb = activePlayer->GetRandomAtkNumber();
      randAtkNb.has_value()) {
    QStringList logTargetAtk;
    const auto randAtkStr =
        activePlayer->FormatStringRandAtk(randAtkNb.value());
    if (randAtkStr.has_value()) {
      logTargetAtk.append(randAtkStr.value());
    }
    // Choose the hero target with the most aggro in case of individual reach
    if (const auto heroAgg = gm->m_PlayersManager->GetHeroMostAggro();
        heroAgg.has_value()) {
      logTargetAtk.append(QString("%1 a le + d'aggro(%2)")
                              .arg(heroAgg->first)
                              .arg(heroAgg->second));
    }
    emit SigUpdateChannelView(activePlayer->m_Name, logTargetAtk.join("\n"),
                              activePlayer->color);
  }

  // Update views
  // Update views after stats changes
  // Players panels views
  UpdateViewAtRoundStart(activePlayer, gm);
  // TODO update channel
  // choice of talent
  // if dead -> choice to take a potion

  return true;
}

void GameDisplay::StartNewTurn() {
  // TODO game state , check if boss is dead
  const auto *gm = Application::GetInstance().m_GameManager.get();
  if (gm == nullptr || gm->m_GameState == nullptr ||
      gm->m_PlayersManager == nullptr) {
    return;
  }

  // Increment turn effects
  gm->m_PlayersManager->IncrementCounterEffect();
  // Reset new round boolean for characters
  gm->m_PlayersManager->ResetIsFirstRound();
  // Apply regen stats
  gm->m_PlayersManager->ApplyRegenStats(characType::Boss);
  gm->m_PlayersManager->ApplyRegenStats(characType::Hero);
  // Update views after stats changes
  emit SigUpdatePlayerPanel({});

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
  UpdateGameStatus(gm->m_GameState);
  // game is just starting at turn 1
  // some first init to do for the views
  if (gm->m_GameState->m_CurrentTurnNb == 1) {
    ui->attak_page->InitTargetsWidget(gm->m_PlayersManager);
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

  // generate output file on game stats
  StatsInGame::GenerateStatsEndGame(OUTPUT_ENDGAME);
}

bool GameDisplay::ProcessAtk(
    const TargetInfo *target, const AttaqueType &currentAtk,
    Character *activatedPlayer, const bool isCrit, const QString &nameChara,
    std::unordered_map<QString, std::vector<effectParam>> &newEffects) {
  const auto &gm = Application::GetInstance().m_GameManager;
  if (target == nullptr) {
    return false;
  }
  if (auto *targetChara = gm->m_PlayersManager->GetActiveCharacterByName(
          target->get_name().data());
      targetChara != nullptr) {
    // is dodging
    // a tank cannot dodge
    if (currentAtk.target == TARGET_ENNEMY && currentAtk.reach == REACH_ZONE &&
        target->get_is_targeted()) {
      const auto &[isDodgingZone, outputsRandnbZone] =
          targetChara->IsDodging(currentAtk);
      if (isDodgingZone) {
        // can dodge ?
        if (targetChara->m_Class != CharacterClass::Tank) {
          emit SigUpdateChannelView(targetChara->m_Name,
                                    FightSoundLog::OutputDodge(
                                        targetChara->m_Name, outputsRandnbZone),
                                    targetChara->color);
            m_FightSound.PlayDodgingSound(targetChara->m_Type ==
                                        characType::Boss);
          return true;
        } else {
          emit SigUpdateChannelView(
              targetChara->m_Name,
              QString("pas d'esquive.(%1)").arg(outputsRandnbZone),
              targetChara->color);
        }
        // process block
        targetChara->ProcessBlock(isDodgingZone);
      }
    }
    // EFFECT
    const auto &[conditionsOk, resultEffects, appliedEffects] =
        activatedPlayer->ApplyAtkEffect(target->get_is_targeted(), currentAtk,
                                        targetChara, isCrit);

    if (!resultEffects.isEmpty()) {
      emit SigUpdateChannelView(
          activatedPlayer->m_Name,
          QString("Sur %1: ").arg(target->get_name().data()) + "\n" +
              resultEffects.join(""),
          activatedPlayer->color);
    }
    if (!conditionsOk) {
      ui->bag_button->setEnabled(true);
      ui->attaque_button->setEnabled(true);
      return false;
    }
    // add applied effect to new effect Table
    newEffects[target->get_name().data()] = appliedEffects;

    // Some bufs on the target are disabled at the end of the atk
    targetChara->ResetBuf(BufTypes::changeByHealValue);
    auto *buf =
        targetChara->m_AllBufs[static_cast<int>(BufTypes::changeByHealValue)];
    if (buf != nullptr) {
      buf->set_is_passive_enabled(false);
    }
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
  auto *activatedPlayer =
      gm->m_PlayersManager->GetActiveCharacterByName(nameChara);
  if (activatedPlayer == nullptr) {
    return;
  }
  // launch atk
  activatedPlayer->m_ActionsDoneInRound++;
  if (activatedPlayer->m_AttakList.count(atkName) == 0) {
    return;
  }
  const auto &currentAtk = activatedPlayer->m_AttakList.at(atkName);
  // Process cost of the atk
  activatedPlayer->ProcessCost(atkName);
  QStringList launchingStr;
  launchingStr.append(QString("lance %1.").arg(atkName));
  // process sound
  m_FightSound.AddToQueue(currentAtk.atkSoundPath);
  // update stats in game info
  activatedPlayer->m_StatsInGame.m_AllAtksInfo[atkName].nbOfUse++;

  // is Dodging
  // a tank cannot dodge
  if (currentAtk.target == TARGET_ENNEMY &&
      currentAtk.reach == REACH_INDIVIDUAL) {
    const auto &[isDodging, plName, outputsRandNb] =
        gm->m_PlayersManager->IsDodging(targetList, currentAtk);
    const auto indivTarget =
        gm->m_PlayersManager->GetActiveCharacterByName(plName);
    if (indivTarget != nullptr &&
        indivTarget->m_Class != CharacterClass::Tank) {
      if (isDodging) {
        launchingStr.append(FightSoundLog::OutputDodge(indivTarget->m_Name,
                                                       outputsRandNb.first()));
        emit SigUpdateChannelView(nameChara, launchingStr.join("\n"),
                                  activatedPlayer->color);
        m_FightSound.PlayDodgingSound(indivTarget->m_Type == characType::Boss);
        return;
      } else {
        launchingStr.append(QString("%1 n'esquive pas.(%2)")
                                .arg(plName)
                                .arg(outputsRandNb.first()));
      }
    }
  }

  // is critical Strike ??
  const auto [isCrit, critRandNb] =
      activatedPlayer->ProcessCriticalStrike(currentAtk);
  QString critStr;
  if (isCrit) {
    critStr = "Coup critique";
      m_FightSound.AddToQueue(CRITICAL_SOUND);
  } else {
    critStr = "Pas de coup critique";
  }
  launchingStr.append(QString("%1 (%2)").arg(critStr).arg(critRandNb));
  emit SigUpdateChannelView(activatedPlayer->m_Name, launchingStr.join("\n"),
                            activatedPlayer->color);

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
    if (!ProcessAtk(*it, currentAtk, activatedPlayer, isCrit, nameChara,
                    newEffects)) {
      return;
    }
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
  activatedPlayer->ResetBuf(BufTypes::boostedByHots);
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

    if (!terminatedEffects.isEmpty()) {
      emit SigUpdateChannelView(activatedPlayer->m_Name,
                                terminatedEffects.join("\n"),
                                activatedPlayer->color);
    }
  }

  // check who is dead!
  const QStringList diedBossList =
      gm->m_PlayersManager->CheckDiedPlayers(characType::Boss);
  for (const auto &dp : diedBossList) {
    emit SigUpdateChannelView("GameState", QString("%1 est mort.").arg(dp));
    // LOOT
    for (const auto *hero : gm->m_PlayersManager->m_HeroesList) {
      if (hero == nullptr) {
        continue;
      }
      // TODO what to do when a boss is dead
      const auto newEquip = gm->m_PlayersManager->LootNewEquipments(dp);
      QStringList logLoot;
      if (!newEquip.empty()) {
        logLoot.append(QString("Loot pour %1:").arg(hero->m_Name));
      }
      for (const auto &e : newEquip) {
        logLoot.append(QString("- %1").arg(e.m_UniqueName));
      }
      std::for_each(newEquip.begin(), newEquip.end(), [&](const Stuff &s) {
        EditStuff es;
        es.m_Stuff = s;
        es.m_BodyPart = s.m_BodyPart;
        es.m_Name = s.m_UniqueName;
        EditStuff::SaveStuffInJson(es, s.m_BodyPart);
      });
      emit SigUpdateChannelView("GameState", logLoot.join('\n'));
    }
    // update view for the equipments
    ApplicationView::GetInstance().GetCharacterWindow()->UpdateViewUseStuff();

    emit SigBossDead(dp);
    ui->add_exp_button->setEnabled(true);
    gm->m_GameState->RemoveDeadPlayerInTurn(dp);
    gm->m_GameState->m_DiedEnnemies[gm->m_GameState->m_CurrentTurnNb].push_back(
        dp);
    if (!dp.isEmpty()) {
        m_FightSound.AddToQueue(KILL_SOUND);
    }
  }
  const QStringList diedHeroesList =
      gm->m_PlayersManager->CheckDiedPlayers(characType::Hero);
  for (const auto &dp : diedHeroesList) {
    emit SigUpdateChannelView("GameState", QString("%1 est mort.").arg(dp));
  }

  // update views of heroes and bosses
  emit SigUpdatePlayerPanel(gm->m_PlayersManager->m_AllEffectsOnGame);
  // update stats view
  emit SigUpdStatsOnSelCharacter(gm->m_PlayersManager->m_SelectedPlayer);

  // Check end of game
  if (gm->m_PlayersManager->m_BossesList.empty()) {
    // update buttons
    // TODO new boss to add ? or new phase ?
    // if yes -> start new turn with new boss
    //      if not end of game!!

    EndOfGame();
    m_FightSound.AddToQueue(VICTORY_SOUND);
  }
  if (gm->m_PlayersManager->m_BossesList.empty()) {
    // update buttons
    // TODO new boss to add ? or new phase ?
    // if yes -> start new turn with new boss
    //      if not end of game!!

    EndOfGame();
    m_FightSound.AddToQueue(VICTORY_SOUND);
  }
  // check game over
  if (gm->m_PlayersManager->CheckGameOver()) {
    EndOfGame();
      m_FightSound.AddToQueue(GAME_OVER_SOUND);
  }
}

void GameDisplay::on_mana_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_MANA);
      m_FightSound.AddToQueue(POTION_SOUND);
    emit SigUpdatePlayerPanel({});
  }
}

void GameDisplay::on_hp_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_HP);
      m_FightSound.AddToQueue(POTION_SOUND);
    emit SigUpdatePlayerPanel({});
  }
}

void GameDisplay::on_berseck_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_BERSECK);
      m_FightSound.AddToQueue(POTION_SOUND);
    emit SigUpdatePlayerPanel({});
  }
}

void GameDisplay::on_vigor_potion_button_clicked() {
  auto *hero = Application::GetInstance().m_GameManager->GetCurrentPlayer();
  if (hero != nullptr) {
    hero->UsePotion(STATS_VIGOR);
      m_FightSound.AddToQueue(POTION_SOUND);
    emit SigUpdatePlayerPanel({});
  }
}

void GameDisplay::on_add_exp_button_clicked() {
  Application::GetInstance().m_GameManager->m_PlayersManager->AddExpForHeroes(
      ui->exp_spinBox->value());
  // update level + exp label of each hero panel
  emit SigUpdatePlayerPanel({});
  emit selectCharacter(Application::GetInstance()
                           .m_GameManager->m_PlayersManager->m_SelectedPlayer);
  // update atk view
  ui->attak_page->UpdateActions(ActionsStackedWgType::attak);
}

void GameDisplay::SlotUpdateActionViews(const QString &name,
                                        const QString &form) {
  const auto &gm = Application::GetInstance().m_GameManager;
  const auto &nameChara = gm->m_GameState->GetCurrentPlayerName();
  if (nameChara != name) {
    return;
  }
  auto *activatedPlayer =
      gm->m_PlayersManager->GetActiveCharacterByName(nameChara);
  if (activatedPlayer == nullptr) {
    return;
  }
  if (name == "Thalia") {
    activatedPlayer->SetValuesForThalia(form == BEAR_FORM);
    emit SigUpdatePlayerPanel({});
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

void GameDisplay::UpdateActivePlayers(const bool isLoadingGame,
                                      const GameManager *gm) {
  if (gm == nullptr) {
    return;
  }
  auto *c = gm->m_PlayersManager->m_SelectedPlayer;
  emit SigGameDisplayStart(c);
  emit selectCharacter(c);
  // reset at load game
  if (isLoadingGame) {
    UpdateAtLoadGame(gm);
  }
}

void GameDisplay::ResetUi() {
  ui->bosses_widget->ResetUi();
  ui->heroes_widget->ResetUi();
  ui->channel_lay->ResetUi();
  ui->attak_page->ResetUi();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::defaultType));
}

void GameDisplay::UpdateAtLoadGame(const GameManager *gm) {
  if (gm == nullptr || gm->m_PlayersManager == nullptr ||
      gm->m_PlayersManager->m_SelectedPlayer == nullptr) {
    return;
  }
  // the selected player is the player to start playing at loading
  ui->attak_page->InitTargetsWidget(gm->m_PlayersManager);
  UpdateViewAtRoundStart(gm->m_PlayersManager->m_SelectedPlayer, gm);
  const bool endOfRound =
      gm->m_PlayersManager->m_SelectedPlayer->m_ActionsDoneInRound ==
      gm->m_PlayersManager->m_SelectedPlayer->m_MaxNbActionsInRound;
  if (endOfRound &&
      gm->m_GameState->m_CurrentRound < gm->m_GameState->m_OrderToPlay.size()) {
    ui->channel_lay->UpdateTurnsButtons(false);
    ui->stackedWidget->setCurrentIndex(
        static_cast<int>(ActionsStackedWgType::defaultType));
    ui->attaque_button->setEnabled(false);
    ui->bag_button->setEnabled(false);
  } else {
    ui->channel_lay->UpdateTurnsButtons(true);
  }
}

void GameDisplay::UpdateViewAtRoundStart(Character *activePlayer,
                                         const GameManager *gm) {
  if (activePlayer == nullptr || gm == nullptr ||
      gm->m_PlayersManager == nullptr || gm->m_GameState == nullptr) {
    return;
  }
  ui->heroes_widget->ActivatePanel(activePlayer->m_Name);
  ui->bosses_widget->ActivatePanel(activePlayer->m_Name);
  // Activate actions buttons
  ui->bag_button->setEnabled(true);
  ui->attaque_button->setEnabled(true);
  // actions views
  ui->attak_page->SetCurrentPlayer(activePlayer);
  // set focus on active player
  emit SigSetFocusOnActivePlayer(activePlayer);
  emit SigUpdatePlayerPanel(gm->m_PlayersManager->m_AllEffectsOnGame);
  emit SigUpdateChannelView("GameState",
                            QString("Round %1/%2")
                                .arg(gm->m_GameState->m_CurrentRound)
                                .arg(gm->m_GameState->m_OrderToPlay.size()));
  // auto select hero
  gm->m_PlayersManager->m_SelectedPlayer = activePlayer;
  emit selectCharacter(activePlayer);

  // set atk view on
  // do it after hero has been activated and selected
  ui->attak_page->ResetActionsParam();
  ui->stackedWidget->setCurrentIndex(
      static_cast<int>(ActionsStackedWgType::attak));
  ui->attaque_button->setEnabled(false);
  ui->bag_button->setEnabled(true);
  ui->attak_page->UpdateActions(ActionsStackedWgType::attak);
}
