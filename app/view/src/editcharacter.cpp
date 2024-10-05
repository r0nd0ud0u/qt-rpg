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
    delete ui; }

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
    if (panel->m_MaxValue == -1) {
      panel->m_MaxValue = 0;
    }
    ch->m_Stats.m_AllStatsTable[panel->m_Name].InitValues(panel->m_MaxValue, panel->m_MaxValue);
    // reset panel
    panel->m_MaxValue = -1;
  }
}

void EditCharacter::Init(const Character* c){
    if(c == nullptr){
        return;
    }
    ui->boss_radio->setChecked(c->m_type == characType::Boss);
    ui->boss_radio->setEnabled(c->m_type == characType::Boss);
    ui->hero_radio->setChecked(c->m_type == characType::Hero);
    ui->hero_radio->setEnabled(c->m_type == characType::Hero);
}
