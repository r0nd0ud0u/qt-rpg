#include "bossesview.h"
#include "ui_bossesview.h"

#include "Application.h"
#include "bosspanel.h"

BossesView::BossesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BossesView)
{
    ui->setupUi(this);
    setStyleSheet("#main_widget{ background:     #ff8000;} ");
    InitBossPanels();
}


BossesView::~BossesView()
{
    delete ui;
}

void BossesView::InitBossPanels() {

    const auto &app = Application::GetInstance();
    if (app.m_GameManager != nullptr && app.m_GameManager->m_PlayersManager != nullptr) {
        for (const auto &it : app.m_GameManager->m_PlayersManager->m_BossesList) {
            if(it == nullptr){
                continue;
            }
            auto *bossPanel = new BossPanel();
            bossPanel->UpdatePanel(it);
            ui->main_widget->layout()->addWidget(bossPanel);
            m_BossPanels.push_back(bossPanel);
            bossPanel->SetActive(false);
            //connect(bossPanel, &HeroPanel::addStuff, this, &HeroesView::Dosomething);
        }
        if(!m_BossPanels.empty()){
            m_BossPanels.front()->SetActive(true);
        }
    }
}
