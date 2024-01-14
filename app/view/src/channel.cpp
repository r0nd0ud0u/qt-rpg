#include "channel.h"
#include "ui_channel.h"

#include "heropanel.h"

Channel::Channel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Channel)
{
    ui->setupUi(this);


    // retrueve instance main window and find ui hero panel
   // connect(ui->list_stuffs, &HeroPanel::addStuff, this, &Channel::ShowPageStuffs);
}

Channel::~Channel()
{
    delete ui;
}

void Channel::ShowPageStuffs(){
    ui->stackedWidget->setCurrentIndex(0);
}

void Channel::AddStuff(){
    ui->list_stuffs->AddStuff("test");
}
