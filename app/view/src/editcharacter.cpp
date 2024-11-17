#include "editcharacter.h"
#include "ui_editcharacter.h"

#include "character.h"
#include "common.h"
#include "editstatpanel.h"

EditCharacter::EditCharacter(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditCharacter) {
  ui->setupUi(this);

  InitEditCharacter();
}

EditCharacter::~EditCharacter() {
  for (auto *it : m_PanelList) {
    delete it;
    it = nullptr;
  }
  m_PanelList.clear();
  delete ui;
}

void EditCharacter::InitEditCharacter() {

  for (const auto &stats : ALL_STATS) {
    if (stats.isEmpty()) {
      continue;
    }
    auto *statsPanel = new EditStatPanel();
    statsPanel->m_Name = stats;
    Stats statsDef;
    statsPanel->Init(stats, statsDef);
    ui->stats_scroll->layout()->addWidget(statsPanel);
    m_PanelList.push_back(statsPanel);
  }
}

void EditCharacter::AddCharacter(Character *ch) const {
  if (ch == nullptr) {
    return;
  }
  ch->m_Name = ui->name_edit->toPlainText();
  ch->m_type = characType::Hero;
  if (ui->boss_radio->isChecked()) {
    ch->m_type = characType::Boss;
  }

  for (const auto &panel : m_PanelList) {
      if (panel->m_RawMax == -1) {
          panel->m_RawMax = 0;
    }
    if (panel->m_CurrRaw == -1) {
        panel->m_CurrRaw = 0;
    }
    // update stats character
    ch->m_Stats.m_AllStatsTable[panel->m_Name].InitValues(panel->m_CurrRaw,
                                                          panel->m_RawMax);
    // reset panel
    panel->m_RawMax = -1;
    panel->m_CurrRaw = -1;
  }
  ch->m_IsBlockingAtk = ui->cb_is_blocking_atk->isChecked();
  ch->m_Power.is_crit_heal_after_crit = ui->cb_is_crit_heal_after_crit->isChecked();
  ch->m_Power.is_damage_tx_heal_needy_ally = ui->cb_is_damage_tx_heal_needy_ally->isChecked();
  ch->m_ExtCharacter->set_is_heal_atk_blocked(ui->cb_is_heal_atk_blocked->isChecked());
  ch->m_ExtCharacter->set_is_first_round(ui->cb_is_first_round->isChecked());
  ch->m_ExtCharacter->set_is_random_target(ui->cb_is_random_target->isChecked());
  ch->m_MaxNbActionsInRound = ui->spinBox->value();
}

void EditCharacter::Init(const Character *c) {
  if (c == nullptr) {
    return;
  }
  // update name
  ui->name_edit->setText(c->m_Name);
  // update hero/boss button
  ui->boss_radio->setChecked(c->m_type == characType::Boss);
  ui->boss_radio->setEnabled(c->m_type == characType::Boss);
  ui->hero_radio->setChecked(c->m_type == characType::Hero);
  ui->hero_radio->setEnabled(c->m_type == characType::Hero);
  // init stats value on panel
  for (auto *panel : m_PanelList) {
    if (panel == nullptr) {
      continue;
    }
    const auto &stat = panel->m_Name;
    if (c->m_Stats.m_AllStatsTable.count(stat) == 0) {
      continue;
    }
    panel->SetStatsValue(c->m_Stats.m_AllStatsTable.at(stat));
  }
  ui->cb_is_blocking_atk->setChecked(c->m_IsBlockingAtk);
  ui->cb_is_crit_heal_after_crit->setChecked(c->m_Power.is_crit_heal_after_crit);
  ui->cb_is_damage_tx_heal_needy_ally->setChecked(c->m_Power.is_damage_tx_heal_needy_ally);
  ui->cb_is_heal_atk_blocked->setChecked(c->m_ExtCharacter->get_is_heal_atk_blocked());
  ui->cb_is_first_round->setChecked(c->m_ExtCharacter->get_is_first_round());
  ui->cb_is_random_target->setChecked(c->m_ExtCharacter->get_is_random_target());
  ui->spinBox->setValue(c->m_MaxNbActionsInRound);
}

