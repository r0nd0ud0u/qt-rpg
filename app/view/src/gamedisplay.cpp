#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "heroesview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameDisplay)
{
    ui->setupUi(this);

    connect(ui->heroes_widget, &HeroesView::SigAddStuff, this, &GameDisplay::UpdateChannel);
    connect(ui->heroes_widget, &HeroesView::SigClickedOnHeroPanel, this, &GameDisplay::UpdateViews);

    ui->stackedWidget->setCurrentIndex(0);
}

GameDisplay::~GameDisplay()
{
    delete ui;
}

void GameDisplay::UpdateChannel(){
    ui->channel_lay->ShowPageStuffs();
    ui->channel_lay->AddStuff();
}

void GameDisplay::UpdateViews(const QString& name){
    emit selectCharacter(name);
 }
