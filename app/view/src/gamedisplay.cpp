#include "gamedisplay.h"
#include "ui_gamedisplay.h"

#include "heroesview.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameDisplay)
{
    ui->setupUi(this);

    connect(ui->heroes_widget, &HeroesView::SigAddStuff, this, &GameDisplay::UpdateChannel);


}

GameDisplay::~GameDisplay()
{
    delete ui;
}

void GameDisplay::UpdateChannel(){
    ui->channel_lay->ShowPageStuffs();
    ui->channel_lay->AddStuff();
}
