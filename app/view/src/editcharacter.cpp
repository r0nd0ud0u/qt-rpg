#include "editcharacter.h"
#include "ui_editcharacter.h"

#include "common.h"
#include "editstatpanel.h"
#include "character.h"

EditCharacter::EditCharacter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditCharacter)
{
    ui->setupUi(this);

    InitEditCharacter();
}

EditCharacter::~EditCharacter()
{
    delete ui;
}


void EditCharacter::InitEditCharacter() {

    for(const auto& stats : ALL_STATS){
        if(stats.isEmpty()){
            continue;
        }
        auto* statsPanel = new EditStatPanel();
        statsPanel->m_Name = stats;
        Stats statsDef;
        statsPanel->Init(stats, statsDef);
        ui->stats_scroll->layout()->addWidget(statsPanel);
        m_PanelList.push_back(statsPanel);
    }
}

void EditCharacter::AddCharacter(Character* ch) const{
    if(ch == nullptr){
        return;
    }
    ch->m_Name = ui->name_edit->toPlainText();
    ch->m_type = characType::Hero;
    if(ui->boss_radio->isChecked()){
        ch->m_type = characType::Boss;
    }

    for (const auto& panel : m_PanelList) {
        std::get<StatsType<int>>(ch->m_Stats.m_AllStatsTable[panel->m_Name])
            .InitValues(panel->m_StartingValue,panel->m_StartingValue, panel->m_MaxValue, panel->m_RegenValue);

    }
}
